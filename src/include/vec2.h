#pragma once
#include "raylib.h"
#include <cmath>

class Vec2 {
public:
    Vec2() = default;
    Vec2(float _x, float _y);

    Vector2 toRaylib() const;

    float dot(const Vec2& v) const;
    float length() const;
    void normalize();
    void rotate(float angle);

    bool operator==(const Vec2& v) const;
    bool operator!=(const Vec2& v) const;

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

struct MyTransform {
    MyTransform() = default;
    MyTransform(Vec2 pos, Vec2 size, float angle);
    Vec2 pos = Vec2(0, 0);
    Vec2 size = Vec2(0, 0);
    float angle = 0;
    float scale = 1;
};