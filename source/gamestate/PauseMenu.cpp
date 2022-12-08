#include "gamestate/PauseMenu.hpp"

#include "helper/RenderHelper.hpp"

PauseMenu::PauseMenu(MainGame& gameIn) : game(gameIn)
{
    buffer = C2D_TextBufNew(48);
    C2D_TextFontParse(&textA, NULL, buffer, "PAUSE");
    C2D_TextFontParse(&textB, NULL, buffer, "B: Main Menu");
    C2D_TextFontParse(&textS, NULL, buffer, "Start/Select: Back");
    C2D_TextOptimize(&textA);
    C2D_TextOptimize(&textB);
    C2D_TextOptimize(&textS);
    scoreBuf = C2D_TextBufNew(16);
}

PauseMenu::~PauseMenu()
{
    C2D_TextBufDelete(buffer);
    C2D_TextBufDelete(scoreBuf);
}

GameType PauseMenu::UpdateGameState(GameType lastSate, float deltaTime)
{
    if (lastSate != GameType::MENU_PAUSE)
    {
        C2D_TextBufClear(scoreBuf);
        char tmp[16];
        snprintf(tmp, 16, "%lu", game.scores.actualScore);
        C2D_TextFontParse(&scoreText, NULL, scoreBuf, tmp);
        C2D_TextOptimize(&scoreText);
    }
    if (inputs->GetButton(KEY_START |KEY_SELECT, false))
    {
        return GameType::GAME_MAIN;
    }
    if (inputs->GetButton(KEY_B, false))
    {
        return GameType::MENU_MAIN;
    }
    return GameType::MENU_PAUSE;
}

void PauseMenu::Render(bool isTopScreen)
{
    static const u32 color = C2D_Color32f(0.4f, 0.9f, 1.0f, 1.0f);
    if (isTopScreen)
    {
        C2D_DrawText(&textA, C2D_AlignCenter | C2D_WithColor, SCREEN_WIDTH/2 + 40, 40, 0.0f, 2.0f, 2.0f, color);
        float& posX = game.player.spr.params.pos.x;
        float& posY = game.player.spr.params.pos.y;
        Vec2D tmpPos = Vec2D(posX, posY);
        float tmpRot = game.player.spr.params.angle;
        posY = 200;
        game.player.spr.params.angle = 0.0f;
        for (u8 i = 0; i < game.player.life; i++)
        {
            posX = i*40 + 70;
            C2D_DrawSprite(&game.player.spr);
        }
        posX = tmpPos.x;
        posY = tmpPos.y;
        game.player.spr.params.angle = tmpRot;
        C2D_DrawText(&scoreText, C2D_AlignRight | C2D_WithColor, SCREEN_WIDTH + 20, 200, 0.0f, 1.0f, 1.0f, color);
    }
    else
    {
        C2D_DrawText(&textB, C2D_AlignCenter | C2D_WithColor, SCREEN_WIDTH/2, 30, 0.0f, 1.0f, 1.0f, color);
        C2D_DrawText(&textS, C2D_AlignCenter | C2D_WithColor, SCREEN_WIDTH/2, 60, 0.0f, 1.0f, 1.0f, color);
    }
}