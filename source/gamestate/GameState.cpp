#include "gamestate/GameState.hpp"
#include "helper/RenderHelper.hpp"

InputHelper* GameState::inputs;

void GameState::SetInputs(InputHelper* inputIn)
{
    inputs = inputIn;
}

void GameState::TopScreenExtras()
{
    static const u32 clearColorBlack = C2D_Color32(0, 0, 0, 255u);
    C2D_DrawRectSolid(0, 0, 0, 40, 240, clearColorBlack);
	C2D_DrawRectSolid(360, 0, 0, 40, 240, clearColorBlack);
}