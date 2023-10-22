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

double MyAbs(double value) {
    if(value >= 0) {
        return value;
    }
    else {
        return -value;
    }
}

Vec2 Physics::frictionVec() const {
    double fricX = -sign(speed.x) * friction;
    if (MyAbs(fricX) > MyAbs(speed.x)) {
        fricX = -speed.x;
    } else if (MyAbs(speed.x) > maxMoveSpeed) {
        fricX = MyAbs(speed.x) - maxMoveSpeed > MyAbs(fricX) * 10 ? fricX * 10 : -sign(speed.x) * (MyAbs(speed.x) - maxMoveSpeed);
    }
    double fricY = -sign(speed.y) * friction;
    if (MyAbs(fricY) > MyAbs(speed.y)) {
        fricY = -speed.y;
    } else if (speed.y > maxFallSpeed) {
        fricY = speed.y - maxFallSpeed > MyAbs(fricY) * 10 ? fricY * 10 : -(speed.y - maxFallSpeed);
    } else if (speed.y < -maxFlySpeed) {
        fricY = MyAbs(speed.y) - maxFlySpeed > MyAbs(fricY) * 10 ? fricY * 10 : -(speed.y + maxFlySpeed);
    }
    
    return Vec2(fricX, fricY);
}

Vec2& Physics::calcSpeed() {
    speed += accel;
    speed.y += gravity;
    speed += frictionVec();
    return speed;
}