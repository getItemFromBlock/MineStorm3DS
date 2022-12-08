#pragma once

#include <3ds.h>
#include "helper/InputHelper.hpp"

enum class GameType : u8
{
    MENU_MAIN = 0,
    MENU_SCORES,
    MENU_OPTIONS,
    MENU_PAUSE,
    MENU_GAMEOVER,
    MENU_WIN,
    GAME_MAIN,
    GAME_ANIM,
    GAME_EXIT,
};

class GameState
{
protected:
    static InputHelper* inputs;
public:
    virtual GameType UpdateGameState(GameType lastSate, float deltaTime) = 0;
    virtual void Render(bool isTopScreen) = 0;
    virtual void TopScreenExtras();
    static void SetInputs(InputHelper* inputsIn);
};