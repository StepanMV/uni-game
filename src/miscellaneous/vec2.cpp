#include "vec2.h"

Vec2::Vec2(float _x, float _y): x(_x), y(_y) {}

Vector2 Vec2::toRaylib() const {
    return {x, y};
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