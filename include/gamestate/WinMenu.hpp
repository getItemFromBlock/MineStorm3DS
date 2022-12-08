#pragma once

#include <vector>

#include "GameState.hpp"
#include "AnimGame.hpp"
#include "object/particle/StarParticle.hpp"

class WinMenu : public GameState
{
private:
    std::vector<StarParticle> particles;
    AnimGame& game;
    float animTimer = 0.0f;

public:
    WinMenu(AnimGame& gameIn);
    ~WinMenu();

    GameType UpdateGameState(GameType lastSate, float deltaTime) override;
    void Render(bool isTopScreen) override;
};
