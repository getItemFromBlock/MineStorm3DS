#pragma once

#include <citro2d.h>
#include <3ds.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SCREEN_DHEIGHT 2*SCREEN_HEIGHT

class RenderHelper
{
private:
    C3D_RenderTarget* top;
    C3D_RenderTarget* bottom;
    const u32 clearColorA = C2D_Color32f(0.125f, 0.071f, 0.302f, 1.0f);
    const u32 clearColorB = C2D_Color32f(0.094, 0.055f, 0.231f, 1.0f);
    const u32 clearColorC = C2D_Color32f(0.063, 0.035, 0.157, 1.0f);
    const u32 clearColorBlack = C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f);

public:
    RenderHelper();
    ~RenderHelper();
    static void DrawSpriteTiled(C2D_Sprite* spr, bool bottom);
    void BeginTop();
    void BeginBottom();
    void EndRender();
};