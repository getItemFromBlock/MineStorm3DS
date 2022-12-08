#include "object/enemy/MagneticMine.hpp"

#include "gamestate/MainGame.hpp"

#define ENEMIES_ANGULAR_VELOCITY 50.0f / 180.0f * M_PI

static C2D_Image* sources = nullptr;

static const Vec2D MagneticMineHitBox[4][3] = {
    {{ 1.0f, -25.0f}, {51.0f, -51.0f}, {27.0f, 1.0}},
    {{27.0f, 1.0f}, {51.0f, 51.0f}, {1.0f, 27.0f}},
    {{1.0f, 27.0f}, {-51.0f, 51.0f}, {-25.0f, 1.0f}},
    {{-25.0f, 1.0f}, {-51.0f, -51.0f}, {1.0f, -25.0f}}
};

static Poly hitbox[] =
{
    Poly(MagneticMineHitBox[0], 3),
    Poly(MagneticMineHitBox[1], 3),
    Poly(MagneticMineHitBox[2], 3),
    Poly(MagneticMineHitBox[3], 3),
};

Poly* MagneticMine::GetPolys(Vec2D refPos)
{
    float hSize = (size == EnemySize::SIZE_LARGE ? 0.33333333f : (size == EnemySize::SIZE_MEDIUM ? 0.25f : 0.166666666f));
    for (u8 i = 0; i < GetPolyCount(); i++)
    {
        hitbox[i].SetParameters(refPos, hSize, spr.params.angle);
    }
    return hitbox;
}

u8 MagneticMine::GetPolyCount()
{
    return sizeof(hitbox)/sizeof(Poly);
}

MagneticMine::MagneticMine()
{
}


MagneticMine::MagneticMine(const Vec2D& position, float rotation, float velIn, EnemySize sizeIn)
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

MagneticMine::~MagneticMine()
{
}

Enemy* MagneticMine::CreateNew(const Vec2D& position, float rotation, float vel, EnemySize size)
{
    return new MagneticMine(position, rotation, vel, size);
}

void MagneticMine::SetSourceImage(C2D_Image* sourceImg)
{
    sources = sourceImg;
}

u32 MagneticMine::GetScoreValue()
{
    switch (size)
    {
    case EnemySize::SIZE_SMALL:
        return 600;
    case EnemySize::SIZE_MEDIUM:
        return 535;
    default:
        return 500;
    }
}

void MagneticMine::Update(float deltaTime, GameState* holder)
{
    Vec2D pos = Vec2D(spr.params.pos.x, spr.params.pos.y);
    Vec2D pPos = ((MainGame*)holder)->GetPlayerPosition(pos) - pos;
    float dir = GetAngle(pPos);
    float dec = cut(mod(spr.params.angle - dir, -M_PI, M_PI), -ENEMIES_ANGULAR_VELOCITY * deltaTime, ENEMIES_ANGULAR_VELOCITY * deltaTime);
    spr.params.angle -= dec;
    pos = pos + Vec2D(cosf(spr.params.angle), sinf(spr.params.angle)) * deltaTime * vel;
    spr.params.pos.x = pos.x;
    spr.params.pos.y = pos.y;
    Entity::Update(deltaTime, holder);
}

float MagneticMine::GetDefaultVelocity()
{
    return RandomRange(10, 40);
}