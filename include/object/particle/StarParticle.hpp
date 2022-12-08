#pragma once

#include "Particle.hpp"

class StarParticle : public Particle
{
public:
    StarParticle(Vec2D pos, float rot, float speed);
    ~StarParticle() override;

    void Update(float deltaTime) override;
    void Render(bool isTopScreen) override;
};