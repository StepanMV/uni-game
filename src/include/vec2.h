#pragma once
#include "raylib.h"

class Vec2 {
    public:
        Vec2() = default;
        Vec2(float _x, float _y);

        Vector2 toRaylib() const;

        float dot(const Vec2& v) const;
        void normalize();

        Vec2& operator+=(const Vec2& v);
        Vec2& operator-=(const Vec2& v);
        Vec2& operator*=(const float& k);

        ~Vec2() = default;
        float x = 0, y = 0;
};

Vec2 operator+(const Vec2& v1, const Vec2& v2);
Vec2 operator-(const Vec2& v1, const Vec2& v2);
Vec2 operator*(const Vec2& v, const float& k);
Vec2 operator*(const float& k, const Vec2& v);
Vec2 operator-(const Vec2& v);