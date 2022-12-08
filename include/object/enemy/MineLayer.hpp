#pragma once

#include "Enemy.hpp"

class MineLayer : public Enemy
{
protected:
    float timer = 0.0f;
public:
    MineLayer();
    MineLayer(const Vec2D& position, float rotation, float vel, EnemySize size);
    virtual ~MineLayer() override;

    virtual Enemy* CreateNew(const Vec2D& position, float rotation, float vel, EnemySize size) override;
    virtual u32 GetScoreValue() override;
    virtual void Update(float deltaTime, GameState* holder) override;
    float GetDefaultVelocity() override;
    virtual Poly* GetPolys(Vec2D refPos) override;
    virtual u8 GetPolyCount() override;
    static void SetSourceImage(C2D_Image* sourceImg);
};
