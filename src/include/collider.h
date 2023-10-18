#pragma once

#include "vec2.h"
#include <vector>
#include <memory>

class Collider {
public:
    Collider() = delete;
    Collider(Vec2* pos, Vec2* size, float* angle = 0);

    bool MyCheckCollision(const std::shared_ptr<Collider> other) const;

    void setCenterOffset(Vec2 offset);
    void calcHitbox();
private:
    Vec2* pos;
    Vec2* size;
    float* angle;

    std::vector<Vec2> hitbox = std::vector<Vec2>(4);
    Vec2 centerOffset = Vec2(0, 0);

    Vec2 rotatedCenter = Vec2(0, 0);
    std::vector<Vec2> rotatedHitbox = {
        Vec2(+size->x / 2, +size->y / 2),
        Vec2(+size->x / 2, -size->y / 2),
        Vec2(-size->x / 2, -size->y / 2),
        Vec2(-size->x / 2, +size->y / 2)
    };
};