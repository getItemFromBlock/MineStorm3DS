#include "object/particle/ExplosionParticle.hpp"
#include "helper/RenderHelper.hpp"

ExplosionParticle::ExplosionParticle(Vec2D posIn, float rotIn, float speedIn, float timerIn)
{
    pos = posIn;
    vel = Vec2D(cosf(rotIn), sinf(rotIn)) * speedIn;
    lifeTime = timerIn;
    IsValid = true;
}

ExplosionParticle::~ExplosionParticle()
{
}

void ExplosionParticle::Render(bool isTopScreen)
{
    if (lifeTime < 0.0f) return;
    static const u32 color = C2D_Color32f(1.0f, 1.0f, 0.0f, 1.0f);
    s32 decY = isTopScreen ? 0 : -SCREEN_HEIGHT;
    s32 decX = isTopScreen ? 40 : 0;
    Vec2D oldPos = pos;
    for (s8 i = -1; i < 2; i++)
    {
        for (s8 j = -1; j < 2; j++)
        {
            pos = Vec2D(decX, decY) + Vec2D(SCREEN_WIDTH * i, SCREEN_DHEIGHT * j) + oldPos;
            if (pos.x > -16 + decX && pos.y > -16 && pos.x < SCREEN_WIDTH + 16 + decX && pos.y < SCREEN_DHEIGHT + 16)
            {
                C2D_DrawLine(pos.x, pos.y, color, pos.x + vel.x, pos.y + vel.y, color, 1.0f, 0.0f);
            }
        }
    }
    pos = oldPos;
}

void ExplosionParticle::Update(float deltaTime)
{
    lifeTime += deltaTime;
    if (lifeTime <= 0) return;
    pos = pos + vel * deltaTime;
    if (lifeTime > deltaTime)
    {
        vel = vel + (vel / lifeTime) * deltaTime;
    }
    if (lifeTime > 0.5f)
    {
        IsValid = false;
    }
    Particle::Update(deltaTime);
}