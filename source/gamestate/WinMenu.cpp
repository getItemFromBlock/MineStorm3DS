#include "gamestate/WinMenu.hpp"

#include "helper/RenderHelper.hpp"
#include "helper/SoundHelper.hpp"

WinMenu::WinMenu(AnimGame& gameIn) : game(gameIn)
{
}

WinMenu::~WinMenu()
{
}

GameType WinMenu::UpdateGameState(GameType lastSate, float deltaTime)
{
    if (lastSate == GameType::GAME_MAIN)
    {
        SoundHelper::PlayVictorySound();
        animTimer = 3.0f;
        particles.clear();
    }
    animTimer -= deltaTime;
    particles.push_back(StarParticle(Vec2D(SCREEN_WIDTH/2, SCREEN_HEIGHT), RandomRange(0.0f, 2*M_PI), 400));
    for (u32 i = 0; i < particles.size(); i++)
    {
        if (particles[i].IsValid) particles[i].Update(deltaTime);
    }
    if (animTimer <= 0.0f)
    {
        if (game.GetLevel() >= 10)
        {
            return GameType::MENU_GAMEOVER;
        }
        return GameType::GAME_ANIM;
    }
    return GameType::MENU_WIN;
}

void WinMenu::Render(bool isTopScreen)
{
    for (u32 i = 0; i < particles.size(); i++)
    {
        particles[i].Render(isTopScreen);
    }
}