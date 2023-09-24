#include "physics.h"

#include "cmath"
#include "vec2.h"

void Physics::setMaxSpeeds(double maxMoveSpeed, double maxFallSpeed, double maxFlySpeed) {
    this->maxMoveSpeed = maxMoveSpeed;
    this->maxFallSpeed = maxFallSpeed;
    this->maxFlySpeed = maxFlySpeed;
}

void Physics::setForces(double gravity, double friction) {
    this->gravity = gravity;
    this->friction = friction;
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

Vec2 Physics::frictionVec() const {
    double fricX = -sign(speed.x) * friction;
    fricX = abs(fricX) > abs(speed.x) ? -speed.x : fricX;
    double fricY = -sign(speed.y) * friction;
    fricY = abs(fricY) > abs(speed.y) ? -speed.y : fricY;
    return Vec2(fricX, fricY);
}

void Physics::applyAccel() {
    speed += accel;
    speed += frictionVec();
    speed += Vec2(0, gravity);

    if(abs(speed.x) >= maxMoveSpeed) {
        speed.x = sign(speed.x) * maxMoveSpeed;
    }
    if(speed.y > maxFallSpeed) {
        speed.y = maxFallSpeed;
    }
    if(speed.y < -maxFlySpeed) {
        speed.y = -maxFlySpeed;
    }
}