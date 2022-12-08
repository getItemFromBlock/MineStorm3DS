#include "object/TitleSprite.hpp"

#include "helper/RenderHelper.hpp"

TitleSprite::TitleSprite(C2D_SpriteSheet& spriteSheet)
{
    C2D_SpriteFromSheet(&spr, spriteSheet, 0);
	C2D_SpriteSetCenter(&spr, 0.5f, 0.5f);
	C2D_SpriteSetPos(&spr, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    for (u8 i = 0; i < 4; i++)
    {
        tintA.corners[i].blend = 0.5f;
        tintA.corners[i].color = 0xffffffff;
        tintB.corners[i].blend = 0.5f;
        tintB.corners[i].color = 0xff0000ff;
    }
    counter = 0.0f;
}

TitleSprite::~TitleSprite()
{
}

void TitleSprite::Render(bool isTopScreen)
{
    s32 decY = isTopScreen ? 0 : SCREEN_HEIGHT;
    s32 decX = isTopScreen ? 40 : 0;
    spr.params.pos.x += decX;
    spr.params.pos.y -= decY;
    C2D_DrawSpriteTinted(&spr, static_cast<s32>(counter*2)%2 ? &tintB : &tintA);
    spr.params.pos.x -= decX;
    spr.params.pos.y += decY;
}

void TitleSprite::Update(float deltaTime, GameState* holder)
{
    counter += deltaTime;
}