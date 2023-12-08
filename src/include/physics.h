#pragma once

#include "vec2.h"

class Physics {
    public:
        Physics() = default;

        Vec2& calcSpeed();

        Vec2 speed, accel;
        double maxMoveSpeed, maxFallSpeed, maxFlySpeed;
        double friction, gravity;

    private:
        Vec2 frictionVec() const;
};