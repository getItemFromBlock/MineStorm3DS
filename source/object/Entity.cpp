#include "object/Entity.hpp"

#include "helper/RenderHelper.hpp"

void Entity::Render(bool isTopScreen)
{
    s32 decY = isTopScreen ? 0 : -SCREEN_HEIGHT;
    s32 decX = isTopScreen ? 40 : 0;
    float& posX = spr.params.pos.x;
    float& posY = spr.params.pos.y;
    float oldPosX = posX;
    float oldPosY = posY;
    for (s8 i = -1; i < 2; i++)
    {
        for (s8 j = -1; j < 2; j++)
        {
            posX = decX + SCREEN_WIDTH * i + oldPosX;
            posY = decY + SCREEN_DHEIGHT * j + oldPosY;
            if (posX > -64 + decX && posY > -64 && posX < SCREEN_WIDTH + 64 + decX && posY < SCREEN_DHEIGHT + 64)
            {
                C2D_DrawSprite(&spr);
            }
        }
    }
    posX = oldPosX;
    posY = oldPosY;
}

void Entity::Update(float deltaTime, GameState* holder)
{
    if (spr.params.pos.x < 0) spr.params.pos.x += SCREEN_WIDTH;
    if (spr.params.pos.x >= SCREEN_WIDTH) spr.params.pos.x -= SCREEN_WIDTH;
    if (spr.params.pos.y < 0) spr.params.pos.y += SCREEN_DHEIGHT;
    if (spr.params.pos.y >= SCREEN_DHEIGHT) spr.params.pos.y -= SCREEN_DHEIGHT;
}