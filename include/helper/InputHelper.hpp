#pragma once

#include <3ds.h>

class InputHelper
{
private:
    bool debugMode = false;
    bool wideScreen = true;
    u32 kDown = 0;
    u32 kHeld = 0;
    circlePosition pos;
public:
    InputHelper();
    ~InputHelper();
    void Update();
    float GetSteer();
    bool GetButton(u32 key, bool held);
    bool IsDebug();
    bool IsWide();
    void ToggleDebug();
    void ToggleWideMode();
};
