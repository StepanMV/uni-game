#include "vec2.h"

Vec2::Vec2(float _x, float _y): x(_x), y(_y) {}

Vector2 Vec2::toRaylib() const {
    return {x, y};
}

float Vec2::dot(const Vec2& v) const {
    return x * v.x + y * v.y;
}

void Vec2::normalize() {
    float length = sqrt(x * x + y * y);
    x /= length;
    y /= length;
}

void Vec2::rotate(float angle) {
    float rad = angle * M_PI / 180;
    float _x = x * cos(rad) - y * sin(rad);
    float _y = x * sin(rad) + y * cos(rad);
    x = _x;
    y = _y;
}

Vec2& Vec2::operator+=(const Vec2& v) {
    x += v.x;
    y += v.y;
    return *this;
}

Vec2& Vec2::operator-=(const Vec2& v) {
    x -= v.x;
    y -= v.y;
    return *this;
}

Vec2& Vec2::operator*=(const float& k) {
    x *= k;
    y *= k;
    return *this;
}

Vec2 Vec2::fromScreenToWorld() const
{
    return Vec2(this->x + GetMouseX() - GetScreenWidth() / 2, this->y + GetMouseY() - GetScreenHeight() / 2);
}

Vec2 operator+(const Vec2& v1, const Vec2& v2) {
    return Vec2(v1.x + v2.x, v1.y + v2.y);
}

Vec2 operator-(const Vec2& v1, const Vec2& v2) {
    return Vec2(v1.x - v2.x, v1.y - v2.y);
}

Vec2 operator*(const Vec2& v, const float& k) {
    return Vec2(v.x * k, v.y * k);
}

Vec2 operator*(const float& k, const Vec2& v) {
    return Vec2(v.x * k, v.y * k);
}

Vec2 operator-(const Vec2& v) {
    return Vec2(-v.x, -v.y);
}