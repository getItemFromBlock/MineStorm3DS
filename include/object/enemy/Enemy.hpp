#pragma once

#include <citro2d.h>
#include <3ds.h>

#include "object/Entity.hpp"
#include "helper/MathHelper.hpp"
#include "helper/ColliderHelper.hpp"

enum class EnemySize : u8
{
    SIZE_SMALL = 0,
    SIZE_MEDIUM = 1,
    SIZE_LARGE = 2,
};

class EnemySpawner;
class MainGame;

class Enemy : public Entity
{
protected:
    EnemySize size;
    float vel;
    EnemySize GetChildSize();
    bool isActive = false;
    bool canDivide = true;
public:
    virtual ~Enemy();
    virtual u32 GetScoreValue() = 0;
    virtual void KillEnemy(MainGame* game);
    virtual Enemy* CreateNew(const Vec2D& position, float rotation, float vel, EnemySize size) = 0;
    virtual void Render(bool istopScreen) override;
    virtual float GetDefaultVelocity() = 0;
    virtual Poly* GetPolys(Vec2D refPos) = 0;
    virtual u8 GetPolyCount() = 0;

    friend class EnemySpawner;
    friend class MainGame;
};
