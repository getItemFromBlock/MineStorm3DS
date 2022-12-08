#pragma once

#include "GameState.hpp"

#include "object/TitleSprite.hpp"

class MainMenu : public GameState
{
private:
    TitleSprite title;
    C2D_TextBuf buffer;
    C2D_Text textA;
    C2D_Text textB;
    C2D_Text textX;
    C2D_Text textY;
public:
    MainMenu(C2D_SpriteSheet& spriteSheet);
    ~MainMenu();

    GameType UpdateGameState(GameType lastSate, float deltaTime) override;
    void Render(bool isTopScreen) override;
};
