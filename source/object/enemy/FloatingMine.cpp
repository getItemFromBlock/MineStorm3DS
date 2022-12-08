#include "object/enemy/FloatingMine.hpp"

static C2D_Image* sources = nullptr;

static const Vec2D FloatingMineHitBox[3][3] = {
    {{-17.0f, 4.0f}, {1.0f, -56.0f}, {21.0f, 4.0}},
    {{21.0f, 4.0f}, {61.0f, 54.0f}, {1.0f, 40.0f}},
    {{1.0f, 40.0f}, {-59.0f, 54.0f}, {-17.0f, 4.0f}}
};

static Poly hitbox[] =
{
    Poly(FloatingMineHitBox[0], 3),
    Poly(FloatingMineHitBox[1], 3),
    Poly(FloatingMineHitBox[2], 3),
};

Poly* FloatingMine::GetPolys(Vec2D refPos)
{
    float hSize = (size == EnemySize::SIZE_LARGE ? 0.33333333f : (size == EnemySize::SIZE_MEDIUM ? 0.25f : 0.166666666f));
    for (u8 i = 0; i < GetPolyCount(); i++)
    {
        hitbox[i].SetParameters(refPos, hSize, spr.params.angle);
    }
    return hitbox;
}

u8 FloatingMine::GetPolyCount()
{
    return sizeof(hitbox)/sizeof(Poly);
}

FloatingMine::FloatingMine()
{
}

FloatingMine::FloatingMine(const Vec2D& position, float rotation, float velIn, EnemySize sizeIn)
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

FloatingMine::~FloatingMine()
{
}

Enemy* FloatingMine::CreateNew(const Vec2D& position, float rotation, float vel, EnemySize size)
{
    return new FloatingMine(position, rotation, vel, size);
}

void FloatingMine::SetSourceImage(C2D_Image* sourceImg)
{
    sources = sourceImg;
}

u32 FloatingMine::GetScoreValue()
{
    switch (size)
    {
    case EnemySize::SIZE_SMALL:
        return 200;
    case EnemySize::SIZE_MEDIUM:
        return 135;
    default:
        return 100;
    }
}

void FloatingMine::Update(float deltaTime, GameState* holder)
{
    if (!isActive) return;
    Vec2D tmp = Vec2D(spr.params.pos.x, spr.params.pos.y);
    tmp = tmp + velocity * deltaTime;
    spr.params.pos.x = tmp.x;
    spr.params.pos.y = tmp.y;
    Entity::Update(deltaTime, holder);
}

float FloatingMine::GetDefaultVelocity()
{
    return RandomRange(30, 50);
}