#include "helper/ColliderHelper.hpp"
#include "helper/RenderHelper.hpp"

Poly::Poly() : Poly(nullptr, 0)
{
}

Poly::Poly(const Vec2D* pvertices, int pCount) : vertices(pvertices),
vCount(pCount), pos(Vec2D()), size(1.0f), rot(0.0f)
{

}

Poly::~Poly()
{
}


void Poly::SetParameters(Vec2D ppos, float psize, float prot)
{
    pos = ppos;
    size = psize;
    rot = prot;
}

void Poly::SetPosition(Vec2D ppos)
{
    pos = ppos;
}

ProjectionResult Poly::project(Vec2D axis) const
{
    RotationMatrix2D rm = RotationMatrix2D(rot);
    float f = (rm.applyRotation(vertices[0]*size)+pos).dotProduct(axis);
    ProjectionResult res = {f, f};
    for (int i = 1; i < vCount; i++) {
        f = (rm.applyRotation(vertices[i]*size)+pos).dotProduct(axis);
        if (f < res.min) res.min = f;
        if (f > res.max) res.max = f;
    }
    return res;
}

bool Poly::collide(Poly& in) const
{
    for (int i = 0; i < vCount; i++) {
        Vec2D normal = vertices[i];
        normal = normal.getNormal();
        ProjectionResult pr1 = project(normal);
        ProjectionResult pr2 = in.project(normal);
        if (pr2.min > pr1.max || pr1.min > pr2.max) return false;
    }
    for (int i = 0; i < in.vCount; i++) {
        Vec2D normal = in.vertices[i];
        normal = normal.getNormal();
        ProjectionResult pr1 = in.project(normal);
        ProjectionResult pr2 = project(normal);
        if (pr2.min > pr1.max || pr1.min > pr2.max) return false;
    }
    return true;
}

bool Poly::collideMultiple(Poly* in, int pCount) const
{
    for (int i = 0; i < pCount; i++) {
        if (collide(in[i])) return true;
    }
    return false;
}

void Poly::RenderPoly(bool isTopScreen)
{
    static const u32 color = C2D_Color32f(1.0f, 0.0f, 0.0f, 1.0f);
    RotationMatrix2D rm = RotationMatrix2D(rot);
    s32 decY = isTopScreen ? 0 : -SCREEN_HEIGHT;
    s32 decX = isTopScreen ? 40 : 0;
    Vec2D last = rm.applyRotation(vertices[0])*size;
    for (s8 i = 0; i < vCount+1; i++)
    {
        Vec2D tmp = rm.applyRotation(vertices[(i+1)%vCount])*size;
        C2D_DrawLine(pos.x + tmp.x + decX, pos.y + tmp.y + decY, color,
        pos.x + last.x + decX, pos.y + last.y + decY, color, 1.0f, 0.0f);
        last = tmp;
    }
}