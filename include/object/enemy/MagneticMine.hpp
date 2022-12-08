#pragma once

#include "Enemy.hpp"

class MagneticMine : public Enemy
{
private:
    /* data */
public:
    MagneticMine();
    MagneticMine(const Vec2D& position, float rotation, float vel, EnemySize size);
    virtual ~MagneticMine() override;

    virtual Enemy* CreateNew(const Vec2D& position, float rotation, float vel, EnemySize size) override;
    virtual u32 GetScoreValue() override;
    virtual void Update(float deltaTime, GameState* holder) override;
    float GetDefaultVelocity() override;
    virtual Poly* GetPolys(Vec2D refPos) override;
    virtual u8 GetPolyCount() override;
    static void SetSourceImage(C2D_Image* sourceImg);
};
