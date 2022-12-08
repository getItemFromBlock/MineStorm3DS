#include "helper/RenderHelper.hpp"

RenderHelper::RenderHelper()
{
	//gfxSetWide(true);
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	//consoleInit(GFX_BOTTOM, NULL);
    top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
}

RenderHelper::~RenderHelper()
{
}

void RenderHelper::BeginTop()
{
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, clearColorBlack);
	C2D_SceneBegin(top);
	C2D_DrawRectangle(40, 0, 0, 320, 240, clearColorA, clearColorA, clearColorB, clearColorB);
}

void RenderHelper::BeginBottom()
{
	C2D_TargetClear(bottom, clearColorBlack);
	C2D_SceneBegin(bottom);
	C2D_DrawRectangle(0, 0, 0, 320, 240, clearColorB, clearColorB, clearColorC, clearColorC);
}

void RenderHelper::EndRender()
{
	C3D_FrameEnd(0);
}