#include "helper/InputHelper.hpp"

InputHelper::InputHelper(/* args */)
{
}

InputHelper::~InputHelper()
{
}

void InputHelper::Update()
{
	hidScanInput();
	kDown = hidKeysDown();
	kHeld = hidKeysHeld();
	hidCircleRead(&pos);
	if (pos.dx > 140)
		pos.dx = 140;
	if (pos.dx < -140)
		pos.dx = -140;
	if (pos.dx > -15 && pos.dx < 15)
		pos.dx = 0;
}

float InputHelper::GetSteer()
{
	return pos.dx / 140.0f;
}

bool InputHelper::GetButton(u32 key, bool held)
{
	if (held)
	{
		return kHeld & key;
	}
	else
	{
		return kDown & key;
	}
}

bool InputHelper::IsDebug()
{
	return debugMode;
}

bool InputHelper::IsWide()
{
	return wideScreen;
}

void InputHelper::ToggleDebug()
{
	debugMode = !debugMode;
}

void InputHelper::ToggleWideMode()
{
	wideScreen = !wideScreen;
}