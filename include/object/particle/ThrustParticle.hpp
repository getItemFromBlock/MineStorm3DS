#pragma once

#include "Particle.hpp"

class ThrustParticle : public Particle
{
public:
    ThrustParticle(const Vec2D& pos, float rot, float speed);
    ~ThrustParticle() override;

    void Update(float deltaTime) override;
    void Render(bool isTopScreen) override;
};