#include "object/enemy/MineLayer.hpp"

#include "gamestate/MainGame.hpp"

static C2D_Image* img = nullptr;

static const Vec2D MinelayerHitBoxCenter[4] = {
    {-105.0f,-10.0f}, {107.0f,-10.0f}, {107.0f,16.0f}, {-105.0f,16.0f}
};

static const Vec2D MinelayerHitBoxBorder[3][3] = {
    {{-33.0f, -10.0f}, {0.0f, -38.0f}, {33.0f, -10.0}},
    {{-105.0f, 16.0f}, {-127.0f, 41.0f}, {-105.0f, -10.0f}},
    {{107.0f, -10.0f}, {129.0f, 41.0f}, {107.0f, 16.0f}}
};

static Poly hitbox[] =
{
    Poly(MinelayerHitBoxCenter, 4),
    Poly(MinelayerHitBoxBorder[0], 3),
    Poly(MinelayerHitBoxBorder[1], 3),
    Poly(MinelayerHitBoxBorder[2], 3),
};

Poly* MineLayer::GetPolys(Vec2D refPos)
{
    for (u8 i = 0; i < GetPolyCount(); i++)
    {
        hitbox[i].SetParameters(refPos, 0.25f, spr.params.angle);
    }
    return hitbox;
}

u8 MineLayer::GetPolyCount()
{
    return sizeof(hitbox)/sizeof(Poly);
}

MineLayer::MineLayer()
{
    canDivide = false;
}


MineLayer::MineLayer(const Vec2D& position, float rotation, float velIn, EnemySize sizeIn)
{
    size = sizeIn;
    C2D_SpriteFromImage(&spr, *img);
    C2D_SpriteSetCenter(&spr, 0.5f, 0.5f);
    spr.params.pos.x = position.x;
    spr.params.pos.y = position.y;
    spr.params.angle = 0.0f;
    vel = velIn;
    canDivide = false;
}

MineLayer::~MineLayer()
{
}

Enemy* MineLayer::CreateNew(const Vec2D& position, float rotation, float vel, EnemySize size)
{
    return new MineLayer(position, rotation, vel, size);
}

void MineLayer::SetSourceImage(C2D_Image* sourceImg)
{
    img = sourceImg;
}

u32 MineLayer::GetScoreValue()
{
    return 1000;
}

void MineLayer::Update(float deltaTime, GameState* holder)
{
    if (!isActive) return;
    s64 timerInt = static_cast<s64>(timer);
    timer += deltaTime;
    float angle = M_PI_4 * sinf(timer*M_PI) - M_PI_2;
    Vec2D pos = Vec2D(spr.params.pos.x, spr.params.pos.y);
    pos = pos + Vec2D(cosf(angle), sinf(angle)) * deltaTime * vel;
    spr.params.pos.x = pos.x;
    spr.params.pos.y = pos.y;
    s64 timerInt2 = static_cast<s64>(timer);
    Entity::Update(deltaTime, holder);
    if (timerInt != timerInt2 && timerInt2 <= 30 && timerInt2 % 3 == 0)
    {
        MainGame* game = static_cast<MainGame*>(holder);
        Enemy* obj = game->GetRandomEnemy();
        if (!obj) return;
        obj = obj->CreateNew(pos, RandomRange(0.0f, 2*M_PI), obj->GetDefaultVelocity()*2.25f, EnemySize::SIZE_SMALL);
        game->AddEnemy(obj);
        std::vector<EnemySpawner>& spawners = game->GetSpawners();
        {
            for (u32 i = 0; i < spawners.size(); i++)
            {
                if (!spawners[i].IsValid)
                {
                    spawners[i].spr.params.pos.x = pos.x;
                    spawners[i].spr.params.pos.y = pos.y;
                    spawners[i].Assign(obj);
                    spawners[i].IsValid = true;
                    return;
                }
            }
            spawners.push_back(EnemySpawner());
            spawners.back().spr.params.pos.x = pos.x;
            spawners.back().spr.params.pos.y = pos.y;
            spawners.back().Assign(obj);
        }
    }
}

float MineLayer::GetDefaultVelocity()
{
    return RandomRange(70, 120);
}