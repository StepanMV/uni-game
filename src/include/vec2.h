#pragma once
#include "raylib.h"

class Vec2 {
    public:
        Vec2() = default;
        Vec2(double _x, double _y);

        Vector2 toRaylib();

        Vec2& operator+=(const Vec2& v);
        Vec2& operator-=(const Vec2& v);
        Vec2& operator*=(const double& k);

        ~Vec2() = default;
        double x = 0, y = 0;
};

Vec2 operator+(const Vec2& v1, const Vec2& v2);
Vec2 operator-(const Vec2& v1, const Vec2& v2);
Vec2 operator*(const Vec2& v, const double& k);
Vec2 operator*(const double& k, const Vec2& v);