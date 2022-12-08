#include "helper/MathHelper.hpp"

#include <random>

float Vec2D::lengthSquared() const
{
    return (x * x + y * y);
}

float Vec2D::getLength() const
{
    return sqrtf(lengthSquared());
}

Vec2D Vec2D::operator+ (const Vec2D& a) const
{
    Vec2D res = Vec2D(a.x + this->x, a.y + this->y);
    return res;
}

Vec2D Vec2D::operator- (const Vec2D& a) const
{
    Vec2D res = Vec2D(this->x - a.x, this->y - a.y);
    return res;
}

Vec2D Vec2D::operator* (const Vec2D& a) const
{
    Vec2D res = Vec2D(this->x * a.x, this->y * a.y);
    return res;
}

Vec2D Vec2D::operator* (const float& a) const
{
    Vec2D res = Vec2D(this->x * a, this->y * a);
    return res;
}

Vec2D Vec2D::operator/ (const float& a) const
{
    if (a == 0.0) return operator*(VEC2D_HIGH_VALUE);
    Vec2D res = operator*(1/a);
    return res;
}

const float& Vec2D::operator[] (const int& in) const
{
    if (in & 0x1) return y;
    return x;
}

float& Vec2D::operator[] (const int& in)
{
    if (in & 0x1) return y;
    return x;
}

bool Vec2D::isCollinearWith(Vec2D a) const
{
    float res = a.x * y - a.y * x;
    return (res < VEC2D_COLLINEAR_PRECISION);
}

float Vec2D::dotProduct(Vec2D a) const
{
    return (a.x * x + a.y * y);
}

float Vec2D::crossProduct(Vec2D a) const
{
    return (x * a.y - y* a.x);
}

Vec2D Vec2D::unitVector() const
{
    return operator/(getLength());
}

Vec2D Vec2D::negate() const
{
    return operator*(-1);
}

Vec2D Vec2D::getNormal() const
{
    return Vec2D(-y, x);
}

bool Vec2D::isIntEquivalent(Vec2D a) const
{
    int x1 = (int)(x>=0?x:x-1);
    int y1 = (int)(y>=0?y:y-1);
    int x2 = (int)a.x;
    int y2 = (int)a.y;
    return (x1 == x2 && y1 == y2);
}

RotationMatrix2D::RotationMatrix2D() : RotationMatrix2D(0)
{
}

RotationMatrix2D::RotationMatrix2D(float angle)
{
    float tmpCos = cosf(angle);
    float tmpSin = sinf(angle);
    content[0][0] = tmpCos;
    content[0][1] = -tmpSin;
    content[1][0] = tmpSin;
    content[1][1] = tmpCos;
}

Vec2D RotationMatrix2D::applyRotation(Vec2D in) const
{
    Vec2D res;
    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 2; i++) {
            res[j] += in[i]*content[j][i];
        }
    }
    return res;
}

inline float toRadians(float in)
{
    return in/180.f*M_PI;
}

inline float toDegrees(float in)
{
    return in*180.f/M_PI;
}

float cut(float in, float min, float max)
{
    if (in < min) in = min;
    if (in > max) in = max;
    return in;
}

float mod(float in, float min, float max)
{
    while(in < min) in += max-min;
    while (in > max) in -= max-min;
    return in;
}

float RandomRange(float min, float max)
{
    return static_cast<float>(rand()) / RAND_MAX * (max - min) + min;
}

float GetAngle(Vec2D point)
{
    float dir;
    if (point.x == 0)
    {
        if (point.y > 0)
            dir = M_PI_2;
        else
            dir = -M_PI_2;
    }
    else
    {
        dir = atanf(point.y / point.x);
        if (point.x < 0)
            dir += M_PI;
    }
    return dir;
}