#pragma once

#include "vec2.h"
#include <vector>
#include <memory>

class Collider {
public:
    Collider(std::shared_ptr<MyTransform> _transform);

    bool MyCheckCollision(const std::shared_ptr<Collider> other) const;

    void setCenterOffset(Vec2 offset);
    void calcHitbox();
private:
    std::shared_ptr<MyTransform> transform;
    std::vector<Vec2> hitbox = std::vector<Vec2>(4);
    Vec2 centerOffset = Vec2(0, 0);

    Vec2 rotatedCenter = Vec2(0, 0);
    std::vector<Vec2> rotatedHitbox = {
        Vec2(+transform->size.x / 2, +transform->size.y / 2),
        Vec2(+transform->size.x / 2, -transform->size.y / 2),
        Vec2(-transform->size.x / 2, -transform->size.y / 2),
        Vec2(-transform->size.x / 2, +transform->size.y / 2)
    };
};