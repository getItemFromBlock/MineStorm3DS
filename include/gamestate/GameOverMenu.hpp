#pragma once

#include "GameState.hpp"
#include "AnimGame.hpp"
#include "object/Scores.hpp"

class GameOverMenu : public GameState
{
private:
    Scores& scores;
    AnimGame& game;
    C2D_TextBuf bufferS;
    C2D_TextBuf bufferA;
    C2D_Text textT;
    C2D_Text textY;
    C2D_Text textS;
    C2D_Text textN;
    C2D_Text textA;
    SwkbdState swkbd;
	SwkbdStatusData swkbdStatus;
    SwkbdButton button = SWKBD_BUTTON_NONE;
    bool isWin = false;
    bool isNew = false;

public:
    GameOverMenu(Scores& scoreIn, AnimGame& gameIn);
    ~GameOverMenu();

    GameType UpdateGameState(GameType lastSate, float deltaTime) override;
    void Render(bool isTopScreen) override;
};
