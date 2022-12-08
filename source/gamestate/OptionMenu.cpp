#include "gamestate/OptionMenu.hpp"

#include "helper/RenderHelper.hpp"
#include "helper/SoundHelper.hpp"

OptionMenu::OptionMenu(Scores& scoreIn) : scores(scoreIn)
{
    buffer = C2D_TextBufNew(80);
    C2D_TextFontParse(&textT, NULL, buffer, "OPTIONS");
    C2D_TextFontParse(&textA, NULL, buffer, "A: Toggle 2DS mode");
    C2D_TextFontParse(&textX, NULL, buffer, "X: Delete scores");
    C2D_TextFontParse(&textY, NULL, buffer, "Y: Toggle debug mode");
    C2D_TextFontParse(&textB, NULL, buffer, "B: Back");
    C2D_TextOptimize(&textT);
    C2D_TextOptimize(&textA);
    C2D_TextOptimize(&textB);
    C2D_TextOptimize(&textX);
    C2D_TextOptimize(&textY);
}

OptionMenu::~OptionMenu()
{
    C2D_TextBufDelete(buffer);
}

GameType OptionMenu::UpdateGameState(GameType lastSate, float deltaTime)
{
    if (inputs->GetButton(KEY_B, false))
    {
        return GameType::MENU_MAIN;
    }
    if (inputs->GetButton(KEY_X, false))
    {
        scores.ResetScores();
    }
    if (inputs->GetButton(KEY_Y, false))
    {
        inputs->ToggleDebug();
    }
    if (inputs->GetButton(KEY_A, false))
    {
        inputs->ToggleWideMode();
    }
    if (inputs->GetButton(KEY_R, false))
    {
        SoundHelper::PlayAmogusSound();
    }
    return GameType::MENU_OPTIONS;
}

void OptionMenu::Render(bool isTopScreen)
{
    static const u32 color = C2D_Color32f(1.0f, 0.9f, 0.4f, 1.0f);
    if (isTopScreen)
    {
        C2D_DrawText(&textT, C2D_AlignCenter | C2D_WithColor, SCREEN_WIDTH/2 + 40, 80, 0.0f, 2.0f, 2.0f, color);
    }
    else
    {
        C2D_DrawText(&textA, C2D_AlignCenter | C2D_WithColor, SCREEN_WIDTH/2, 30, 0.0f, 1.0f, 1.0f, color);
        C2D_DrawText(&textX, C2D_AlignCenter | C2D_WithColor, SCREEN_WIDTH/2, 60, 0.0f, 1.0f, 1.0f, color);
        C2D_DrawText(&textY, C2D_AlignCenter | C2D_WithColor, SCREEN_WIDTH/2, 90, 0.0f, 1.0f, 1.0f, color);
        C2D_DrawText(&textB, C2D_AlignCenter | C2D_WithColor, SCREEN_WIDTH/2, 120, 0.0f, 1.0f, 1.0f, color);
    }
}