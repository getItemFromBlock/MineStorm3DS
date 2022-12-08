#include "helper/SoundHelper.hpp"
#include <atomic>
#include "object/sound/SoundObject.hpp"

/*
 * Fast, threaded Opus audio streaming example using libopusfile
 * for libctru on Nintendo 3DS
 * 
 *     Originally written by Lauren Kelly (thejsa) with lots of help
 * from mtheall, who re-architected the decoding and buffer logic to be
 * much more efficient as well as overall making the code half decent :)
 * 
 *     Thanks also to David Gow for his example code, which is in the
 * public domain & explains in excellent detail how to use libopusfile:
 * https://davidgow.net/hacks/opusal.html
 * 
 * Last update: 2020-05-16
 */

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

// ---- DEFINITIONS ----
static const int SAMPLE_RATE = 48000;            // Opus is fixed at 48kHz
static const int SAMPLES_PER_BUF = SAMPLE_RATE * 120 / 1000;  // 120ms buffer
static const int CHANNELS_PER_SAMPLE = 2;        // We ask libopusfile for
                                                 // stereo output; it will down
                                                 // -mix for us as necessary.

static const int THREAD_AFFINITY = -1;           // Execute thread on any core
static const int THREAD_STACK_SZ = 32 * 1024;    // 32kB stack for audio thread

static const size_t WAVEBUF_SIZE = SAMPLES_PER_BUF * CHANNELS_PER_SAMPLE
    * sizeof(int16_t);                           // Size of NDSP wavebufs

// ---- END DEFINITIONS ----

LightEvent s_event;
ndspWaveBuf s_waveBufs[3];
int16_t *s_audioBuffer = NULL;
std::atomic<bool> s_quit;

static SoundObject effects[6];

// ---- HELPER FUNCTIONS ----

// Retrieve strings for libopusfile errors
// Sourced from David Gow's example code: https://davidgow.net/files/opusal.cpp

const char* SoundHelper::opusStrError(int error)
{
    switch(error) {
        case OP_FALSE:
            return "OP_FALSE: A request did not succeed.";
        case OP_HOLE:
            return "OP_HOLE: There was a hole in the page sequence numbers.";
        case OP_EREAD:
            return "OP_EREAD: An underlying read, seek or tell operation "
                   "failed.";
        case OP_EFAULT:
            return "OP_EFAULT: A NULL pointer was passed where none was "
                   "expected, or an internal library error was encountered.";
        case OP_EIMPL:
            return "OP_EIMPL: The stream used a feature which is not "
                   "implemented.";
        case OP_EINVAL:
            return "OP_EINVAL: One or more parameters to a function were "
                   "invalid.";
        case OP_ENOTFORMAT:
            return "OP_ENOTFORMAT: This is not a valid Ogg Opus stream.";
        case OP_EBADHEADER:
            return "OP_EBADHEADER: A required header packet was not properly "
                   "formatted.";
        case OP_EVERSION:
            return "OP_EVERSION: The ID header contained an unrecognised "
                   "version number.";
        case OP_EBADPACKET:
            return "OP_EBADPACKET: An audio packet failed to decode properly.";
        case OP_EBADLINK:
            return "OP_EBADLINK: We failed to find data we had seen before or "
                   "the stream was sufficiently corrupt that seeking is "
                   "impossible.";
        case OP_ENOSEEK:
            return "OP_ENOSEEK: An operation that requires seeking was "
                   "requested on an unseekable stream.";
        case OP_EBADTIMESTAMP:
            return "OP_EBADTIMESTAMP: The first or last granule position of a "
                   "link failed basic validity checks.";
        default:
            return "Unknown error.";
    }
}

// NDSP audio frame callback
// This signals the audioThread to decode more things
// once NDSP has played a sound frame, meaning that there should be
// one or more available waveBufs to fill with more data.
void audioCallback(void *const nul_) {
    (void)nul_;  // Unused

    if(s_quit.load()) { // Quit flag
        return;
    }
    
    LightEvent_Signal(&s_event);
}

// ---- END HELPER FUNCTIONS ----

// Audio initialisation code
// This sets up NDSP and our primary audio buffer

SoundHelper::SoundHelper()
{
    // Setup LightEvent for synchronisation of audioThread
    LightEvent_Init(&s_event, RESET_ONESHOT);
    // Open the Opus audio file
    int error = 0;
    char array[26] = "romfs:/audio/music_0.opus";
    for (u8 i = 0; i < MUSIC_COUNT; i++)
    {
        array[19] = 'a' + i;
        musics[i] = op_open_file(array, &error);
        if(error) {
            printf("Failed to open file %s: error %d (%s)\n", array, error, opusStrError(error));
        }
    }
}

SoundHelper::~SoundHelper()
{
}

bool SoundHelper::audioInit(void) {
    // Setup NDSP
    ndspChnReset(0);
    ndspSetOutputMode(NDSP_OUTPUT_STEREO);
    ndspChnSetInterp(0, NDSP_INTERP_POLYPHASE);
    ndspChnSetRate(0, SAMPLE_RATE);
    ndspChnSetFormat(0, NDSP_FORMAT_STEREO_PCM16);

    s_quit.store(false);

    // Allocate audio buffer
    const size_t bufferSize = WAVEBUF_SIZE * ARRAY_SIZE(s_waveBufs);
    s_audioBuffer = (int16_t *)linearAlloc(bufferSize);
    if(!s_audioBuffer) {
        printf("Failed to allocate audio buffer\n");
        return false;
    }

    // Setup waveBufs for NDSP
    memset(&s_waveBufs, 0, sizeof(s_waveBufs));
    int16_t *buffer = s_audioBuffer;

    for(size_t i = 0; i < ARRAY_SIZE(s_waveBufs); ++i) {
        s_waveBufs[i].data_vaddr = buffer;
        s_waveBufs[i].status     = NDSP_WBUF_DONE;

        buffer += WAVEBUF_SIZE / sizeof(buffer[0]);
    }
    effects[0].InitSound(1,"romfs:/audio/amogus.opus");
    effects[1].InitSound(2,"romfs:/audio/damage.opus");
    effects[2].InitSound(3,"romfs:/audio/game_over.opus");
    effects[3].InitSound(4,"romfs:/audio/hit.opus");
    effects[4].InitSound(5,"romfs:/audio/shoot.opus");
    effects[5].InitSound(6,"romfs:/audio/victory.opus");
    return true;
}

// Audio de-initialisation code
// Stops playback and frees the primary audio buffer
void SoundHelper::audioExit(void) {
    ndspChnReset(0);
    ndspChnReset(1);
    linearFree(s_audioBuffer);
}

// Main audio decoding logic
// This function pulls and decodes audio samples from opusFile_ to fill waveBuf_
bool fillBuffer(OggOpusFile *opusFile_, ndspWaveBuf *waveBuf_) {
    #ifdef DEBUG
    // Setup timer for performance stats
    TickCounter timer;
    osTickCounterStart(&timer);
    #endif  // DEBUG
    // Decode samples until our waveBuf is full
    int totalSamples = 0;
    while(totalSamples < SAMPLES_PER_BUF) {
        int16_t *buffer = waveBuf_->data_pcm16 + (totalSamples *
            CHANNELS_PER_SAMPLE);
        const size_t bufferSize = (SAMPLES_PER_BUF - totalSamples) *
            CHANNELS_PER_SAMPLE;
        // Decode bufferSize samples from opusFile_ into buffer,
        // storing the number of samples that were decoded (or error)
        const int samples = op_read_stereo(opusFile_, buffer, bufferSize);
        if(samples <= 0) {
            if(samples == 0) break;  // No error here

            printf("op_read_stereo: error %d (%s)", samples,
                   SoundHelper::opusStrError(samples));
            break;
        }
        
        totalSamples += samples;
    }

    // If no samples were read in the last decode cycle, we're done
    if(totalSamples == 0) {
        return false;
    }

    // Pass samples to NDSP
    waveBuf_->nsamples = totalSamples;
    ndspChnWaveBufAdd(0, waveBuf_);
    DSP_FlushDataCache(waveBuf_->data_pcm16,
        totalSamples * CHANNELS_PER_SAMPLE * sizeof(int16_t));

    #ifdef DEBUG
    // Print timing info
    osTickCounterUpdate(&timer);
    printf("fillBuffer %lfms in %lfms\n", totalSamples * 1000.0 / SAMPLE_RATE,
           osTickCounterRead(&timer));
    #endif  // DEBUG

    return true;
}

void SoundHelper::DecodeAllFile(OggOpusFile *opusFile_, s16 *buffer, s64 pcmCount) {
    s64 totalSamples = 0;
    while(totalSamples < pcmCount) {
        // Decode bufferSize samples from opusFile_ into buffer,
        // storing the number of samples that were decoded (or error)
        const int samples = op_read_stereo(opusFile_, buffer+totalSamples*2, (pcmCount-totalSamples)*2);
        if(samples <= 0) {
            if(samples == 0) break;  // No error here

            printf("op_read_stereo: error %d (%s)", samples,
                   opusStrError(samples));
            break;
        }
        totalSamples += samples;
    }
}

u8 getRandomSound(u8 last)
{
    u8 result;
    do {
        result = static_cast<u8>(static_cast<s64>(rand()) * MUSIC_COUNT / RAND_MAX);
        if (result >= MUSIC_COUNT) result = MUSIC_COUNT-1;
    }
    while (result == last);
    return result;
}

// Audio thread
// This handles calling the decoder function to fill NDSP buffers as necessary
void audioThread(void *const files) {
    srand(time(NULL));
    OggOpusFile **const opusFiles = (OggOpusFile**)files;
    u8 selectedSound = getRandomSound(0xffu);
    while(!s_quit.load()) {  // Whilst the quit flag is unset,
                      // search our waveBufs and fill any that aren't currently
                      // queued for playback (i.e, those that are 'done')
        for(size_t i = 0; i < ARRAY_SIZE(s_waveBufs); ++i) {
            if(s_waveBufs[i].status != NDSP_WBUF_DONE) {
                continue;
            }
            
            if(!fillBuffer(opusFiles[selectedSound], &s_waveBufs[i])) {   // Playback complete
                op_pcm_seek(opusFiles[selectedSound], 0);
                selectedSound = getRandomSound(selectedSound);
            }
        }

        // Wait for a signal that we're needed again before continuing,
        // so that we can yield to other things that want to run
        // (Note that the 3DS uses cooperative threading)
        LightEvent_Wait(&s_event);
    }
}

void SoundHelper::spawnAudioThread()
{
    ndspSetCallback(audioCallback, NULL);
    // Spawn audio thread

    // Set the thread priority to the main thread's priority ...
    int32_t priority = 0x30;
    svcGetThreadPriority(&priority, CUR_THREAD_HANDLE);
    // ... then subtract 1, as lower number => higher actual priority ...
    priority -= 1;
    // ... finally, clamp it between 0x18 and 0x3F to guarantee that it's valid.
    priority = priority < 0x18 ? 0x18 : priority;
    priority = priority > 0x3F ? 0x3F : priority;

    // Start the thread, passing our opusFile as an argument.
    threadId = threadCreate(audioThread, musics,
                                         THREAD_STACK_SZ, priority,
                                         THREAD_AFFINITY, false);
    printf("Created audio thread %p\n", threadId);
}

void SoundHelper::exitAudioThread()
{

    // Signal audio thread to quit
    s_quit.store(true);
    LightEvent_Signal(&s_event);

    // Free the audio thread
    threadJoin(threadId, UINT64_MAX);
    threadFree(threadId);

    // Cleanup audio things and de-init platform features
    audioExit();
    ndspExit();
    for (u8 i = 0; i < MUSIC_COUNT; i++)
    {
        op_free(musics[i]);
    }

}

void SoundHelper::PlayHitSound()
{
    effects[3].PlaySound();
}

void SoundHelper::PlayShootSound()
{
    effects[4].PlaySound();
}

void SoundHelper::PlayDamageSound()
{
    effects[1].PlaySound();
}

void SoundHelper::PlayGameOverSound()
{
    effects[2].PlaySound();
}

void SoundHelper::PlayVictorySound()
{
    effects[5].PlaySound();
}

void SoundHelper::PlayAmogusSound()
{
    effects[0].PlaySound();
}