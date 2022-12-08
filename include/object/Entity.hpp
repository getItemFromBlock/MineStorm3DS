#pragma once

#include <citro2d.h>

class GameState;

class Entity
{
protected:
    C2D_Sprite spr;
public:
    bool IsValid = true;
    virtual void Update(float deltaTime, GameState* holder);
    virtual void Render(bool isTopScreen);
};