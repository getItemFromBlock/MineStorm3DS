#pragma once

#include "helper/MathHelper.hpp"

class Particle
{
protected:
    Vec2D pos;
    Vec2D vel;
    float lifeTime;
public:
    bool IsValid = false;
    virtual ~Particle() = 0;
    virtual void Update(float deltaTime);
    virtual void Render(bool isTopScreen) = 0;
};