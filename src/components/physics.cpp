#include "physics.h"

#include "cmath"
#include "vec2.h"

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

Vec2& Physics::applyAccel() {
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
    return speed;
}