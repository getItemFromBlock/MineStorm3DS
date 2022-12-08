#include "object/enemy/EnemySpawner.hpp"

#include "helper/RenderHelper.hpp"

C2D_ImageTint EnemySpawner::tint = {
    C2D_Color32f(1.0f, 0.0f, 0.0f, 1.0f), 0.8f,
    C2D_Color32f(1.0f, 0.0f, 0.0f, 1.0f), 0.8f,
    C2D_Color32f(1.0f, 0.0f, 0.0f, 1.0f), 0.8f,
    C2D_Color32f(1.0f, 0.0f, 0.0f, 1.0f), 0.8f
};
C2D_Image* EnemySpawner::img = nullptr;

EnemySpawner::EnemySpawner()
{
    C2D_SpriteFromImage(&spr, *img);
    C2D_SpriteSetCenter(&spr, 0.5f, 0.5f);
    spr.params.pos.x = RandomRange(0.0f, SCREEN_WIDTH-1);
    spr.params.pos.y = RandomRange(0.0f, SCREEN_DHEIGHT-1);
}

EnemySpawner::~EnemySpawner()
{
}

void EnemySpawner::Update(float deltaTime, GameState* holder)
{
    if (assigned)
    {
        spawnCooldown -= deltaTime;
        if (spawnCooldown <= 0.0f)
        {
            IsValid = false;
            assigned->spr.params.pos.x = spr.params.pos.x;
            assigned->spr.params.pos.y = spr.params.pos.y;
            assigned->isActive = true;
        }
    }
}

void EnemySpawner::Render(bool isTopScreen)
{
    s32 decY = isTopScreen ? 0 : SCREEN_HEIGHT;
    s32 decX = isTopScreen ? 40 : 0;
    spr.params.pos.x += decX;
    spr.params.pos.y -= decY;
    if (assigned && spawnCooldown < 3.0f)
    {
        bool even = static_cast<s32>(spawnCooldown*3) % 2;
        if (even)
            C2D_DrawSprite(&spr);
        else
            C2D_DrawSpriteTinted(&spr, &tint);
    }
    else
    {
        C2D_DrawSprite(&spr);
    }
    spr.params.pos.x -= decX;
    spr.params.pos.y += decY;
}

void EnemySpawner::Assign(Enemy* ptr)
{
    spawnCooldown = RandomRange(4.0f, 10.0f);
    assigned = ptr;
}

void EnemySpawner::SetSourceImage(C2D_Image* sourceImg)
{
    img = sourceImg;
}