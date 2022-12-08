#pragma once

#include "Particle.hpp"

class ExplosionParticle : public Particle
{
public:
    ExplosionParticle(Vec2D pos, float rot, float speed, float timer);
    ~ExplosionParticle() override;

    void Update(float deltaTime) override;
    void Render(bool isTopScreen) override;
};