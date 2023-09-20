#pragma once

#include "vec2.h"

class Physics {
    public:
        Physics(Vec2* pos);

        void useGravity(bool enable);
        void applyForce(Vec2& force);
        void setAccel(Vec2& accel);
        void addAccel(Vec2& accel);
        //void target(int& x, int& y);
        void apply();

        ~Physics();
    private:
        Vec2* pos;
        Vec2 speed;
        Vec2 accel;
        bool gravity = true;
};