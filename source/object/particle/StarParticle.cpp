#include "object/particle/StarParticle.hpp"
#include "helper/RenderHelper.hpp"

StarParticle::StarParticle(Vec2D posIn, float rotIn, float speedIn)
{
    pos = posIn;
    vel = Vec2D(cosf(rotIn), sinf(rotIn)) * speedIn;
    IsValid = true;
}

StarParticle::~StarParticle()
{
}

void StarParticle::Render(bool isTopScreen)
{
    static const u32 color = C2D_Color32f(1.0f, 1.0f, 0.0f, 1.0f);
    s32 decY = isTopScreen ? 0 : -SCREEN_HEIGHT;
    s32 decX = isTopScreen ? 40 : 0;
    C2D_DrawCircleSolid(pos.x + decX, pos.y + decY, 0.0f, 1.5f, color);
}

void StarParticle::Update(float deltaTime)
{
    pos = pos + vel * deltaTime;
    if (pos.x < -2 || pos.x > SCREEN_WIDTH+2 || pos.y < -2 || pos.y > SCREEN_DHEIGHT+2)
    {
        IsValid = false;
    }
}