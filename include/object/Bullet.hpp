#pragma once

#include <citro2d.h>
#include <3ds.h>

#include "helper/MathHelper.hpp"
#include "helper/ColliderHelper.hpp"
#include "Entity.hpp"

class MainGame;

class Bullet : public Entity
{
private:
    Vec2D vel;
    float lifeTime;
    bool isPlayerSpawned;
    C2D_ImageTint tint;
    static C2D_Image* img;
public:
    Bullet();
    Bullet(float rotation, float x, float y, bool playerSpawned);
    ~Bullet();

    void Update(float deltaTime, GameState* holder) override;
    void Render(bool isTopScreen) override;
    Poly* GetPoly();
    static void SetSourceImage(C2D_Image* sourceImg);

    friend class MainGame;
};