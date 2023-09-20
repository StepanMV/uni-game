#pragma once

#include "vec2.h"

class Physics {
    public:
        Physics(Vec2* pos, double friction = 1, double gravity = 1);

        void applyForce(Vec2& force);
        void setAccel(Vec2& accel);
        void addAccel(Vec2& accel);
        void setFriction(double friction);
        void setGravity(double gravity);
        //void target(int& x, int& y);
        void apply();

        ~Physics();
    private:
        Vec2* pos;
        Vec2 speed;
        Vec2 accel;
        double maxMoveSpeed, maxFallSpeed, maxFlySpeed;
        double friction, gravity;
};