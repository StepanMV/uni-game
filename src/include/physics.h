#pragma once

#include "vec2.h"

class Physics {
    public:
        Physics() = default;

        void setMaxSpeeds(double maxMoveSpeed, double maxFallSpeed, double maxFlySpeed);
        void setForces(double gravity, double friction);

        //void target(int& x, int& y);
        void applyAccel();

        Vec2 speed, accel;
        bool onGround, fly;

    private:
        Vec2 frictionVec() const;
        double maxMoveSpeed, maxFallSpeed, maxFlySpeed;
        double friction, gravity;
};