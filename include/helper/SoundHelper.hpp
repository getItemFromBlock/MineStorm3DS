#pragma once

#include <opusfile.h>
#include <3ds.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MUSIC_COUNT 6

class SoundHelper
{
private:
    OggOpusFile *musics[MUSIC_COUNT];
    Thread threadId;

    void audioExit(void);

public:
    SoundHelper();
    ~SoundHelper();

    bool audioInit(void);
    void spawnAudioThread();
    void exitAudioThread();
    static const char* opusStrError(int error);
    static void DecodeAllFile(OggOpusFile *opusFile_, s16 *buffer, s64 pcmCount);
    static void PlayHitSound();
    static void PlayShootSound();
    static void PlayDamageSound();
    static void PlayGameOverSound();
    static void PlayVictorySound();
    static void PlayAmogusSound();
};