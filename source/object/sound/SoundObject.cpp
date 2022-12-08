#include "object/sound/SoundObject.hpp"
#include "helper/SoundHelper.hpp"

#define SAMPLE_RATE 48000

SoundObject::SoundObject()
{
}

void SoundObject::InitSound(u8 chIn, const char* path)
{
    channel = chIn;
    int error;
    OggOpusFile* file = op_open_file(path, &error);
    if(error) {
        printf("Failed to open file: error %d (%s)\n", error, SoundHelper::opusStrError(error));
        return;
    }
    ndspChnReset(channel);
    ndspChnSetInterp(channel, NDSP_INTERP_POLYPHASE);
    ndspChnSetRate(channel, SAMPLE_RATE);
    ndspChnSetFormat(channel, NDSP_FORMAT_STEREO_PCM16);

    sSize = op_pcm_total(file, -1);
    if (sSize <= 0)
    {
        printf("Error reading file size\n");
        return;
    }
    memset(&buffer, 0, sizeof(ndspWaveBuf));
    buffer.data_pcm16 = (s16*)linearAlloc(2*sSize*sizeof(s16));
    if (!buffer.data_pcm16)
    {
        printf("Error allocating %llu bytes (%lu available)\n", static_cast<u64>(2*sSize*sizeof(s16)), linearSpaceFree());
        return;
    }
    SoundHelper::DecodeAllFile(file, buffer.data_pcm16, sSize);
    buffer.status = NDSP_WBUF_DONE;
    buffer.nsamples = sSize;
    op_free(file);
}

SoundObject::~SoundObject()
{
    if (buffer.data_pcm16) linearFree(buffer.data_pcm16);
}

void SoundObject::PlaySound()
{
    ndspChnWaveBufClear(channel);
    ndspChnWaveBufAdd(channel, &buffer);
    DSP_FlushDataCache(buffer.data_pcm16, 2 * sSize * sizeof(int16_t));
}