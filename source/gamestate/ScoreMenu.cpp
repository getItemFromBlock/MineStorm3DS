#include "gamestate/ScoreMenu.hpp"

#include "helper/RenderHelper.hpp"

ScoreMenu::ScoreMenu(Scores& scoreIn) : scores(scoreIn)
{
    buffer = C2D_TextBufNew(16);
    C2D_TextFontParse(&textB, NULL, buffer, "B: Back");
    C2D_TextOptimize(&textB);
}

ScoreMenu::~ScoreMenu()
{
    C2D_TextBufDelete(buffer);
}

GameType ScoreMenu::UpdateGameState(GameType lastSate, float deltaTime)
{
    if (inputs->GetButton(KEY_B, false))
    {
        return GameType::MENU_MAIN;
    }
    return GameType::MENU_SCORES;
}

void ScoreMenu::Render(bool isTopScreen)
{
    static const u32 color = C2D_Color32f(1.0f, 0.9f, 0.4f, 1.0f);
    static const u32 colorA = C2D_Color32f(1.0f, 0.4f, 0.9f, 1.0f);
    static const u32 colorB = C2D_Color32f(0.6f, 1.0f, 0.6f, 1.0f);
    if (isTopScreen)
    {
        for (u8 t = 0; t < SCORE_MAX_SIZE; t++)
        {
            C2D_DrawText(&scores.names[t], C2D_AlignLeft | C2D_WithColor, 60, 8 + 12*t, 0.0f, 0.5f, 0.5f, colorA);
            C2D_DrawText(&scores.values[t], C2D_AlignLeft | C2D_WithColor, 260, 8 + 12*t, 0.0f, 0.5f, 0.5f, colorB);
        }
    }
    else
    {
        C2D_DrawText(&textB, C2D_AlignCenter | C2D_WithColor, SCREEN_WIDTH/2, 30, 0.0f, 1.0f, 1.0f, color);
    }
}