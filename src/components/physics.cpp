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
    if (abs(fricX) > abs(speed.x)) {
        fricX = -speed.x;
    } else if (abs(speed.x) > maxMoveSpeed) {
        fricX = abs(speed.x) - maxMoveSpeed > abs(fricX) * 10 ? fricX * 10 : -sign(speed.x) * (abs(speed.x) - maxMoveSpeed);
    }
    double fricY = -sign(speed.y) * friction;
    if (abs(fricY) > abs(speed.y)) {
        fricY = -speed.y;
    } else if (speed.y > maxFallSpeed) {
        fricY = speed.y - maxFallSpeed > abs(fricY) * 10 ? fricY * 10 : -(speed.y - maxFallSpeed);
    } else if (speed.y < -maxFlySpeed) {
        fricY = abs(speed.y) - maxFlySpeed > abs(fricY) * 10 ? fricY * 10 : -(speed.y + maxFlySpeed);
    }
    
    return Vec2(fricX, fricY);
}

Vec2& Physics::calcSpeed() {
    speed += accel;
    speed.y += gravity;
    speed += frictionVec();
    return speed;
}