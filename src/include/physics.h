#pragma once

#include "vec2.h"

class Physics {
    public:
        Physics(Vec2* _pos, double _maxMoveSpeed, double _maxFallSpeed, double _maxFlySpeed,  double _friction = 1, double _gravity = 1);

        void setSpeed(Vec2& _speed);
        void addSpeed(Vec2& _speed);
        void setAccel(Vec2& _accel);
        void addAccel(Vec2& _accel);
        void setFriction(double _friction);
        void setGravity(double _gravity);
        //void target(int& x, int& y);
        void apply();

        ~Physics();
    private:
        Vec2* pos;
        Vec2 speed = Vec2();
        Vec2 accel = Vec2();
        double maxMoveSpeed, maxFallSpeed, maxFlySpeed;
        double friction, gravity;
};