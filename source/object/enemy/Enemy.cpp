#include "object/enemy/Enemy.hpp"

#include "gamestate/MainGame.hpp"
#include "helper/SoundHelper.hpp"

Enemy::~Enemy()
{
}

EnemySize Enemy::GetChildSize()
{
    if (size == EnemySize::SIZE_LARGE)
    {
        return EnemySize::SIZE_MEDIUM;
    }
    return EnemySize::SIZE_SMALL;
}

void Enemy::KillEnemy(MainGame* game)
{
    u8 num = static_cast<char>(size);
    game->AddToScore((3-num)*GetScoreValue());
    game->CreateExplosion(spr.params.pos.x, spr.params.pos.y);
    if (canDivide && size != EnemySize::SIZE_SMALL)
    {
        for (u8 i = 0; i < 2; i++)
        {
            EnemySpawner& spawner = game->GetFirstSpawner();
            Enemy* child = CreateNew(Vec2D(), RandomRange(0.0f, 2*M_PI), vel * 1.5f, GetChildSize());
            game->AddEnemy(child);
            spawner.Assign(child);
        }
    }
    SoundHelper::PlayHitSound();
    IsValid = false;
}

void Enemy::Render(bool isTopScreen)
{
    if (!isActive) return;
    Entity::Render(isTopScreen);
}