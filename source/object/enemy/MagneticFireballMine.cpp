#include "object/enemy/MagneticFireballMine.hpp"

#include "gamestate/MainGame.hpp"

static C2D_Image* sources = nullptr;

static const Vec2D MagneticFireballMineHitBox[4][3] = {
    {{-42.0f, 48.0f}, {-42.0f, -77.0f}, {8.0f, 47.0}},
    {{-45.0f, -42.0f}, {77.0f, -42.0f}, {-45.0f, 8.0f}},
    {{44.0f, -45.0f}, {44.0f, 77.0f}, {-8.0f, -45.0f}},
    {{48.0f, 45.0f}, {-75.0f, 45.0f}, {47.0f, -8.0f}}
};

static Poly hitbox[] =
{
    Poly(MagneticFireballMineHitBox[0], 3),
    Poly(MagneticFireballMineHitBox[1], 3),
    Poly(MagneticFireballMineHitBox[2], 3),
    Poly(MagneticFireballMineHitBox[3], 3),
};

Poly* MagneticFireballMine::GetPolys(Vec2D refPos)
{
    float hSize = (size == EnemySize::SIZE_LARGE ? 0.3 : (size == EnemySize::SIZE_MEDIUM ? 0.225f : 0.15));
    for (u8 i = 0; i < GetPolyCount(); i++)
    {
        hitbox[i].SetParameters(refPos, hSize, spr.params.angle);
    }
    return hitbox;
}

u8 MagneticFireballMine::GetPolyCount()
{
    return sizeof(hitbox)/sizeof(Poly);
}

MagneticFireballMine::MagneticFireballMine()
{
}

MagneticFireballMine::MagneticFireballMine(const Vec2D& position, float rotation, float velIn, EnemySize sizeIn)
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
    vel = velIn;
}

MagneticFireballMine::~MagneticFireballMine()
{
}

Enemy* MagneticFireballMine::CreateNew(const Vec2D& position, float rotation, float vel, EnemySize size)
{
    return new MagneticFireballMine(position, rotation, vel, size);
}

void MagneticFireballMine::SetSourceImage(C2D_Image* sourceImg)
{
    sources = sourceImg;
}

u32 MagneticFireballMine::GetScoreValue()
{
    switch (size)
    {
    case EnemySize::SIZE_SMALL:
        return 850;
    case EnemySize::SIZE_MEDIUM:
        return 750;
    default:
        return 585;
    }
}

void MagneticFireballMine::KillEnemy(MainGame* game)
{
    Enemy::KillEnemy(game);
    Vec2D pos = Vec2D(spr.params.pos.x, spr.params.pos.y);
    Vec2D pPos = game->GetPlayerPosition(pos) - pos;
    float dir = GetAngle(pPos);
    game->SpawnBullet(false, dir + RandomRange(-0.1f, 0.0f), pos.x, pos.y);
    game->SpawnBullet(false, dir + RandomRange(0.0f, 0.1f), pos.x, pos.y);
}