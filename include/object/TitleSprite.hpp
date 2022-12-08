#pragma once

#include <citro2d.h>

#include "Entity.hpp"

class TitleSprite : public Entity
{
private:
    C2D_ImageTint tintA;
    C2D_ImageTint tintB;
    float counter;
public:
    TitleSprite(C2D_SpriteSheet& spriteSheet);
    ~TitleSprite();

    void Render(bool isTopScreen) override;
    void Update(float deltaTime, GameState* holder) override;
};
