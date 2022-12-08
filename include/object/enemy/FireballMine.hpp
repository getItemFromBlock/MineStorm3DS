#pragma once

#include "FloatingMine.hpp"

class FireballMine : public FloatingMine
{
protected:
    /* data */
public:
    FireballMine();
    FireballMine(const Vec2D& position, float rotation, float vel, EnemySize size);
    virtual ~FireballMine() override;

    virtual Enemy* CreateNew(const Vec2D& position, float rotation, float vel, EnemySize size) override;
    virtual u32 GetScoreValue() override;
    virtual void KillEnemy(MainGame* game) override;
    virtual Poly* GetPolys(Vec2D refPos) override;
    virtual u8 GetPolyCount() override;
    static void SetSourceImage(C2D_Image* sourceImg);
};
