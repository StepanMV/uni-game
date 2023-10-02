#pragma once

#include "vec2.h"

class Physics {
    public:
        Physics() = default;

        //void target(int& x, int& y);
        Vec2& calcSpeed();

        Vec2 speed, accel;
        bool onGround, jumping;
        double maxMoveSpeed, maxFallSpeed, maxFlySpeed;
        double friction, gravity;

    private:
        Vec2 frictionVec() const;
};