#pragma once

#include "vec2.h"

class Physics {
    public:
        Physics() = default;

        void setMaxSpeeds(double maxMoveSpeed, double maxFallSpeed, double maxFlySpeed);
        void setForces(double gravity, double friction);

        //void target(int& x, int& y);
        void applyAccel();

        bool checkCollision(const Physics& other) const;

        Vec2 speed, accel;

    private:
        Vec2 frictionVec() const;
        double maxMoveSpeed, maxFallSpeed, maxFlySpeed;
        double friction, gravity;

        bool onGround = false;
};