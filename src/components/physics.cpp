#include "physics.h"
#include "cmath"

Physics::Physics(Vec2* _pos, double _maxMoveSpeed, double _maxFallSpeed, double _maxFlySpeed,  double _friction, double _gravity): 
pos(_pos), 
friction(_friction), 
gravity(_gravity), 
maxMoveSpeed(_maxMoveSpeed), 
maxFallSpeed(_maxFallSpeed), 
maxFlySpeed(_maxFlySpeed) {}

void Physics::setSpeed(Vec2& _speed) {
    speed = _speed;
}

void Physics::addSpeed(Vec2& _speed) {
    speed += _speed;
}

void Physics::setAccel(Vec2& _accel) {
    accel = _accel;
}

void Physics::addAccel(Vec2& _accel) {
    accel += _accel;
}

void Physics::setFriction(double _friction) {
    friction = _friction;
}

void Physics::setGravity(double _gravity) {
    gravity = _gravity;
}

int sign(double a) {
    if(a > 0) {
        return 1;
    }
    else if(a < 0) {
        return -1;
    }
    else {
        return 0;
    }
}

void Physics::apply() {
    speed += accel;
    if(abs(speed.x) >= maxMoveSpeed) {
        speed.x = sign(speed.x) * maxMoveSpeed;
    }
    if(speed.y >= maxFallSpeed) {
        speed.y = maxFallSpeed;
    }
    if(speed.y <= maxFlySpeed) {
        speed.y = maxFlySpeed;
    }
    *pos += speed;
    if(gravity > 0) {
        accel.y += gravity;
    }
    if(friction > 0) {
        if(speed.x != 0) {
            accel.x += (-friction * sign(speed.x));
        }
    }
}