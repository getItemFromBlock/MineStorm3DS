#include "object/Bullet.hpp"

#include "helper/RenderHelper.hpp"

#define BULLET_SPEED 175.0f
#define BULLET_LIFETIME 3.0f

C2D_Image* Bullet::img = nullptr;

static const Vec2D BulletHitBox[6] = {
    {10.0f, -11.0f}, {17.0f, 2.0f}, {10.0f, 15.0f}, {-4.0f, 15.0f}, {-12.0f, 2.0f}, {-6.0f, -11.0f}
};

static Poly hitbox[] = {
    Poly(BulletHitBox, 6),
};

Poly* Bullet::GetPoly()
{
    hitbox->SetParameters(Vec2D(spr.params.pos.x, spr.params.pos.y), 0.25f, 0.0f);
    return hitbox;
}

Bullet::Bullet()
{
    IsValid = false;
    for (u8 i = 0; i < 4; i++)
    {
        tint.corners[i].blend = 0.5f;
        tint.corners[i].color = 0xffff11ff;
    }
}

Bullet::Bullet(float rotation, float x, float y, bool playerSpawned)
{
    C2D_SpriteFromImage(&spr, *img);
    C2D_SpriteSetCenter(&spr, 0.5f, 0.5f);
    spr.params.pos.x = x;
    spr.params.pos.y = y;
    vel = Vec2D(cosf(rotation), sinf(rotation)) * BULLET_SPEED;
    lifeTime = BULLET_LIFETIME;
    isPlayerSpawned = playerSpawned;
    for (u8 i = 0; i < 4; i++)
    {
        tint.corners[i].blend = 0.5f;
        tint.corners[i].color = 0xffff11ff;
    }
}

Bullet::~Bullet()
{
}

void Bullet::Update(float deltaTime, GameState* holder)
{
    lifeTime -= deltaTime;
    if (lifeTime < 0.0f)
    {
        IsValid = false;
    }
    spr.params.pos.x += vel.x * deltaTime;
    spr.params.pos.y += vel.y * deltaTime;
    Entity::Update(deltaTime, holder);
}

void Bullet::SetSourceImage(C2D_Image* sourceImg)
{
    img = sourceImg;
}

void Bullet::Render(bool isTopScreen)
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
                if (isPlayerSpawned) C2D_DrawSprite(&spr);
                else C2D_DrawSpriteTinted(&spr, &tint);
            }
        }
    }
    posX = oldPosX;
    posY = oldPosY;
}