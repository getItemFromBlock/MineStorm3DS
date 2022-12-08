#include "object/enemy/FireballMine.hpp"

#include "gamestate/MainGame.hpp"

static C2D_Image* sources = nullptr;

static const Vec2D FireballMineHitBox[4][3] = {
    {{ 1.0f, -36.0f}, {54.0f, -50.0f}, {39.0f, 1.0}},
    {{39.0f, 1.0f}, {54.0f, 54.0f}, {1.0f, 39.0f}},
    {{1.0f, 39.0f}, {-49.0f, 53.0f}, {-36.0f, 1.0f}},
    {{-36.0f, 1.0f}, {-51.0f, -51.0f}, {1.0f, -36.0f}}
};

static Poly hitbox[] =
{
    Poly(FireballMineHitBox[0], 3),
    Poly(FireballMineHitBox[1], 3),
    Poly(FireballMineHitBox[2], 3),
    Poly(FireballMineHitBox[3], 3),
};

Poly* FireballMine::GetPolys(Vec2D refPos)
{
    float hSize = (size == EnemySize::SIZE_LARGE ? 0.33333333f : (size == EnemySize::SIZE_MEDIUM ? 0.25f : 0.166666666f));
    for (u8 i = 0; i < GetPolyCount(); i++)
    {
        hitbox[i].SetParameters(refPos, hSize, spr.params.angle);
    }
    return hitbox;
}

u8 FireballMine::GetPolyCount()
{
    return sizeof(hitbox)/sizeof(Poly);
}

FireballMine::FireballMine()
{
}

FireballMine::FireballMine(const Vec2D& position, float rotation, float velIn, EnemySize sizeIn)
{
    size = sizeIn;
    switch (size)
    {
    case EnemySize::SIZE_LARGE:
        C2D_SpriteFromImage(&spr, sources[0]);
        break;
    case EnemySize::SIZE_MEDIUM:
        C2D_SpriteFromImage(&spr, sources[1]);
        break;
    default:
        C2D_SpriteFromImage(&spr, sources[2]);
        break;
    }
    C2D_SpriteSetCenter(&spr, 0.5f, 0.5f);
    spr.params.pos.x = position.x;
    spr.params.pos.y = position.y;
    spr.params.angle = rotation;
    velocity = Vec2D(cosf(rotation), sinf(rotation)) * velIn;
    vel = velIn;
}

FireballMine::~FireballMine()
{
}

Enemy* FireballMine::CreateNew(const Vec2D& position, float rotation, float vel, EnemySize size)
{
    return new FireballMine(position, rotation, vel, size);
}

void FireballMine::SetSourceImage(C2D_Image* sourceImg)
{
    sources = sourceImg;
}

u32 FireballMine::GetScoreValue()
{
    switch (size)
    {
    case EnemySize::SIZE_SMALL:
        return 425;
    case EnemySize::SIZE_MEDIUM:
        return 360;
    default:
        return 325;
    }
}

void FireballMine::KillEnemy(MainGame* game)
{
    Enemy::KillEnemy(game);
    Vec2D pos = Vec2D(spr.params.pos.x, spr.params.pos.y);
    Vec2D pPos = game->GetPlayerPosition(pos) - pos;
    float dir = GetAngle(pPos);
    game->SpawnBullet(false, dir, pos.x, pos.y);
}