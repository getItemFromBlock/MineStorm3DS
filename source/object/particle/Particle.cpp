#include "object/particle/Particle.hpp"

#include "helper/RenderHelper.hpp"

Particle::~Particle()
{
}

void Particle::Update(float deltaTime)
{
    if (pos.x < 0) pos.x += SCREEN_WIDTH;
    if (pos.x >= SCREEN_WIDTH) pos.x -= SCREEN_WIDTH;
    if (pos.y < 0) pos.y += SCREEN_DHEIGHT;
    if (pos.y >= SCREEN_DHEIGHT) pos.y -= SCREEN_DHEIGHT;
}