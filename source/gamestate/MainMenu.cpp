#include "gamestate/MainMenu.hpp"

#include "helper/RenderHelper.hpp"

MainMenu::MainMenu(C2D_SpriteSheet& spriteSheet) : title(spriteSheet)
{
    buffer = C2D_TextBufNew(64);
    C2D_TextFontParse(&textA, NULL, buffer, "A: Start game");
    C2D_TextOptimize(&textA);
    C2D_TextFontParse(&textY, NULL, buffer, "Y: Options");
    C2D_TextOptimize(&textY);
    C2D_TextFontParse(&textX, NULL, buffer, "X: Scores");
    C2D_TextOptimize(&textX);
    C2D_TextFontParse(&textB, NULL, buffer, "B: Exit game");
    C2D_TextOptimize(&textB);
}

MainMenu::~MainMenu()
{
    C2D_TextBufDelete(buffer);
}

GameType MainMenu::UpdateGameState(GameType lastSate, float deltaTime)
{
    title.Update(deltaTime, this);
    if (inputs->GetButton(KEY_A, false))
    {
        return GameType::GAME_ANIM;
    }
    if (inputs->GetButton(KEY_Y, false))
    {
        return GameType::MENU_OPTIONS;
    }
    if (inputs->GetButton(KEY_X, false))
    {
        return GameType::MENU_SCORES;
    }
    if (inputs->GetButton(KEY_B, false))
    {
        return GameType::GAME_EXIT;
    }
    return GameType::MENU_MAIN;
}

void MainMenu::Render(bool isTopScreen)
{
    static const u32 color = C2D_Color32f(1.0f, 0.9f, 0.4f, 1.0f);
    if (isTopScreen)
    {
        title.Render(true);
    }
    else
    {
        C2D_DrawText(&textA, C2D_AlignCenter | C2D_WithColor, SCREEN_WIDTH/2, 30, 0.0f, 1.0f, 1.0f, color);
        C2D_DrawText(&textY, C2D_AlignCenter | C2D_WithColor, SCREEN_WIDTH/2, 60, 0.0f, 1.0f, 1.0f, color);
        C2D_DrawText(&textX, C2D_AlignCenter | C2D_WithColor, SCREEN_WIDTH/2, 90, 0.0f, 1.0f, 1.0f, color);
        C2D_DrawText(&textB, C2D_AlignCenter | C2D_WithColor, SCREEN_WIDTH/2, 120, 0.0f, 1.0f, 1.0f, color);
    }
}