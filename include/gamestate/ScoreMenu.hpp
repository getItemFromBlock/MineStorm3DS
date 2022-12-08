#pragma once

#include "GameState.hpp"

#include "object/Scores.hpp"

class ScoreMenu : public GameState
{
private:
    Scores& scores;
    C2D_TextBuf buffer;
    C2D_Text textB;
public:
    ScoreMenu(Scores& scoresIn);
    ~ScoreMenu();

    GameType UpdateGameState(GameType lastSate, float deltaTime) override;
    void Render(bool isTopScreen) override;
};
