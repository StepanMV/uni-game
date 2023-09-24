#pragma once

#include "vec2.h"

class Physics {
    public:
        Physics() = default;
        Physics(Vec2* _pos);

        void setMaxSpeeds(double maxMoveSpeed, double maxFallSpeed, double maxFlySpeed);
        void setForces(double gravity, double friction);

        //void target(int& x, int& y);
        void applyAccel();

        Vec2* pos;
        Vec2 speed;
        Vec2 accel;
        bool onGround = false;
        bool fly = false;

    private:
        Vec2 frictionVec() const;
        double maxMoveSpeed, maxFallSpeed, maxFlySpeed;
        double friction, gravity;
};