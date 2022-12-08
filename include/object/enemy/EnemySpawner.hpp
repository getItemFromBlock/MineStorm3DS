#pragma once

#include <citro2d.h>
#include <3ds.h>

#include "object/Entity.hpp"
#include "object/enemy/Enemy.hpp"

class MainGame;
class AnimGame;
class MineLayer;

class EnemySpawner : public Entity
{
private:
    Enemy* assigned = nullptr;
    float spawnCooldown;
    static C2D_ImageTint tint;
    static C2D_Image* img;
public:
    EnemySpawner();
    ~EnemySpawner();

    void Update(float deltaTime, GameState* holder) override;
    void Render(bool isTopScreen) override;
    void Assign(Enemy* ptr);
    static void SetSourceImage(C2D_Image* sourceImg);

    friend class Enemy;
    friend class MainGame;
    friend class AnimGame;
    friend class MineLayer;
};
