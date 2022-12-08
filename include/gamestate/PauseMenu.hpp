#pragma once

#include "GameState.hpp"
#include "MainGame.hpp"

#include "object/Scores.hpp"

class PauseMenu : public GameState
{
private:
    C2D_TextBuf buffer;
    C2D_Text textA;
    C2D_Text textB;
    C2D_Text textS;
    C2D_TextBuf scoreBuf;
    C2D_Text scoreText;
    MainGame& game;
public:
    PauseMenu(MainGame& gameIn);
    ~PauseMenu();

    GameType UpdateGameState(GameType lastSate, float deltaTime) override;
    void Render(bool isTopScreen) override;
};
