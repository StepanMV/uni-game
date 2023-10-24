#include "bosses.h"

void KingSlime::update() {
    onBoard();
    if(betweenTpTimer->isDone() && onGround) {          // if can tp and on ground
        if(!tpTimer->isDone()) {                            // if tp is not done
            scale = tpTimer->getTimeLeft() / tpTime;        // scale down relative to time left
            physics->accel.x = 0;                           // stop moving
            return;                                         // don't do anything else
        }
        else {                                          // if tp is done
            betweenTpTimer->reset();                        // reset timer between tps
            collider->setPos(tpPos);                        // set position to tp position
            scale = 1;                                      // return scale to normal
        }
    }
    else {                                              // if can't tp
        tpPos = target->pos;                                // update tp position
        tpTimer->reset();                                   // reset tp timer
    }
    physics->accel.y = 0;
    if(onGround) {
        if(!stayTimer->isDone()) {
            physics->accel.x = 0;
            return;
        }
        else {
            stayTimer->reset();
            jumpCount++;
        }
        jumpCount %= 3;
        if(jumpCount == 0) {
            jumpTime *= 2;
            jumpTimer = Timer::getInstance(jumpTime);
            stayTimer = Timer::getInstance(jumpTime + stayTime);
        }
        else if(jumpCount == 1) {
            jumpTime /= 2;
            jumpTimer = Timer::getInstance(jumpTime);
            stayTimer = Timer::getInstance(jumpTime + stayTime);
        }
        Vec2 direction = (target->pos - transform->pos);
        direction.y = 0;
        direction.normalize();
        direction *= 1.5;
        move(direction);
    }
    jump();
    onGround = false;
    collider->calcHitbox();
}

void KingSlime::render() {
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(this->renderer);
    renderer->setState("idle");
    if (!onGround) {
        renderer->setState("jump");
    }
    renderer->setScale(scale);
}
