#include "object/particle/ThrustParticle.hpp"
#include "helper/RenderHelper.hpp"

ThrustParticle::ThrustParticle(const Vec2D& posIn, float rotIn, float speedIn)
{
    pos = posIn;
    vel = Vec2D(cosf(rotIn), sinf(rotIn)) * speedIn;
    lifeTime = 0.0f;
    IsValid = true;
}

ThrustParticle::~ThrustParticle()
{
}

void ThrustParticle::Render(bool isTopScreen)
{
    static const u32 color = C2D_Color32f(0.7f, 0.7f, 0.7f, 1.0f);
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
                C2D_DrawRectSolid(static_cast<s32>(pos.x) / 2 * 2.0f, static_cast<s32>(pos.y) / 2 * 2.0f, 0.0f, 2.0f, 2.0f, color);
            }
        }
    }
    pos = oldPos;
}

void ThrustParticle::Update(float deltaTime)
{
    pos = pos + vel * deltaTime;
    lifeTime += deltaTime;
    if (lifeTime > 1.5f)
    {
        IsValid = false;
    }
    Particle::Update(deltaTime);
}