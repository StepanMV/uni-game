#include "bosses.h"
#include <iostream>

void Slime::slimeBehavior() {
    physics->accel.y = 0;
    if(onGround) {
        if(!stayTimer->isDone()) {
            physics->accel.x = 0;
            return;
        }
        else {
            jumpCount++;
            jumpCount %= 3;
            if(jumpCount == 0) {
                jumpTime *= 2;
            }
            else if(jumpCount == 1) {
                jumpTime /= 2;
            }
            jumpTimer = Timer::getInstance(jumpTime);
            stayTime = (double) GetRandomValue(5 , 30) / 10;
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
}

void KingSlime::update() {
    onBoard();
    if(betweenTpTimer->isDone() && onGround) {                  // if can tp and on ground
        if(!tpTimer->isDone()) {                                    // if tp is not done
            collider->setScale(currentScale * tpTimer->getTimeLeft() / tpTime);     // scale down relative to time left
            physics->accel.x = 0;                                   // stop moving
            return;                                                 // don't do anything else
        }
        else {                                          // if tp is done
            betweenTpTimer->reset();                        // reset timer between tps
            collider->setPos(tpPos);                        // set position to tp position
            collider->setScale(currentScale);                          // return scale to normal
            onGround = false;                           
        }
    }
    else {                                              // if can't tp
        tpPos = target->pos;                                // update tp position
        tpTimer->reset();                                   // reset tp timer
    }
    if(health >= max_health / 1.41) {
        currentScale = (double) health / max_health;
    }
    if(slimeSpanwTimer->isDone()) {
        int slimeCount = GetRandomValue(1, 6);
        for(int i = 0; i < slimeCount; i++) {
            EnemyBuilder::spawn(EnemyType::Slime, 
            transform->pos + Vec2(GetRandomValue(-transform->size.x / 3, transform->size.x / 3), GetRandomValue(-transform->size.y / 3, transform->size.y / 3)), target);
        }
        slimeSpanwTimer->reset();
    }
    collider->setScale(currentScale);
    slimeBehavior();
    collider->calcHitbox();
}

void Slime::render() {
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(this->renderer);
    renderer->setState("idle");
    if (!onGround) {
        renderer->setState("jump");
    }
    renderer->setScale(transform->scale);
}

void Slime::update() {
    onBoard();
    slimeBehavior();
    collider->calcHitbox();
}
