#pragma once

#include "MathHelper.hpp"

struct ProjectionResult
{
    float min;
    float max;
};

class Poly
{
private:
    const Vec2D* vertices;
    int vCount;
    Vec2D pos;
    float size;
    float rot;
public:
    Poly();
    Poly(const Vec2D* pvertices, int pCount);
    ~Poly();

    void SetParameters(Vec2D ppos, float psize, float prot);
    void SetPosition(Vec2D ppos);
    ProjectionResult project(Vec2D axis) const;
    bool collide(Poly &in) const;
    bool collideMultiple(Poly* in, int pCount) const;
    void RenderPoly(bool isTopScreen);
};