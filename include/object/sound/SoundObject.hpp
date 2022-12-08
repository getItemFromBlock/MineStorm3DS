#pragma once

#include <opusfile.h>
#include <3ds.h>

class SoundObject
{
private:
    ndspWaveBuf buffer;
    s64 sSize;
    u8 channel;
public:
    SoundObject();
    ~SoundObject();

    void PlaySound();
    void InitSound(u8 chIn, const char* path);
};
