#include "vec2.h"

Vec2::Vec2(double _x, double _y): x(_x), y(_y) {}

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

Vec2& Vec2::operator*=(const double& k) {
    x *= k;
    y *= k;
    return *this;
}

double Vec2::getX() const {
    return x;
}

double Vec2::getY() const {
    return y;
}

Vec2 operator+(const Vec2& v1, const Vec2& v2) {
    return Vec2(v1.getX() + v2.getX(), v1.getY() + v2.getY());
}

Vec2 operator-(const Vec2& v1, const Vec2& v2) {
    return Vec2(v1.getX() - v2.getX(), v1.getY() - v2.getY());
}

Vec2 operator*(const Vec2& v, const double& k) {
    return Vec2(v.getX() * k, v.getY() * k);
}

Vec2 operator*(const double& k, const Vec2& v) {
    return Vec2(v.getX() * k, v.getY() * k);
}