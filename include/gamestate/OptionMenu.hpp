#pragma once

#include "GameState.hpp"

#include "object/Scores.hpp"

class OptionMenu : public GameState
{
private:
    Scores& scores;
    C2D_TextBuf buffer;
    C2D_Text textT;
    C2D_Text textA;
    C2D_Text textB;
    C2D_Text textX;
    C2D_Text textY;
public:
    OptionMenu(Scores& scoreIn);
    ~OptionMenu();

    GameType UpdateGameState(GameType lastSate, float deltaTime) override;
    void Render(bool isTopScreen) override;
};
