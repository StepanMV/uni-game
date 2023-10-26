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
}

void Slime::update() {
    onBoard();
    slimeBehavior();
    collider->calcHitbox();
}

void EyeOfCtulhu::update() {
    switch(phase) {
        case 1: {
            phase1();
            break;
        }
        case 2: {
            phase2();
            break;
        }
    }
    collider->calcHitbox();
}

void EyeOfCtulhu::phase1() {
    if(health <= max_health / 2) {
        if(!switchTimer) switchTimer = Timer::getInstance(switchTime);
        if(!switchTimer->isDone()) {
            switchPhase();
        }
        else {
            phase = 2;
            chaseTimer->reset();
            switchTimer = nullptr;
        }
        return;
    }
    if(!chaseTimer->isDone()) {
        Vec2 direction = (target->pos - transform->pos);
        if(abs(direction.x) < 5) {
            physics->speed *= 0;
        }
        else {
            physics->speed *= 0.2;
            transform->angle = atan(direction.y / direction.x) * 180 / M_PI;
        }
        direction += Vec2(0, -target->size.y * 5);
        direction.normalize();
        direction *= 0.9;
        move(direction);
        physics->accel *= 0.9;
    }
    else {
        physics->accel = Vec2(0, 0);
        if(dashCount == -1) {
            dashCount = 4;
            chaseTimer->reset();
        }
        if(physics->speed == Vec2(0, 0) && betweenDashesTimer->isDone()) {
            Vec2 direction = target->pos - transform->pos;
            if(dashCount != 0) dash(direction, 30);
            transform->angle = atan(direction.y / direction.x) * 180 / M_PI;
            betweenDashesTimer->reset();
            dashCount--;
        }
    }
}

void EyeOfCtulhu::phase2() {
    if(!chaseTimer->isDone()) {
        Vec2 direction = (target->pos - transform->pos);
        physics->speed *= 0.2;
        transform->angle = atan(direction.y / direction.x) * 180 / M_PI;
        direction += Vec2(0, -target->size.y * 5);
        if(transform->pos.x < target->pos.x) {
            direction -= Vec2(target->size.x * 3, 0);
        }
        else {
            direction += Vec2(target->size.x * 3, 0);
        }
        direction.normalize();
        direction *= 0.9;
        move(direction);
        physics->accel *= 0.9;
    }
    else {
        physics->accel = Vec2(0, 0);
        physics->friction = 0;
        if(dashCount == -1) {
            dashCount = 4;
            chaseTimer->reset();
            physics->friction = 0.5;
        }
        if(dashTimer->isDone()) {
            Vec2 direction = target->pos - transform->pos;
            transform->angle = atan(direction.y / direction.x) * 180 / M_PI;
            if(dashCount != 0) dash(direction, 20);
            dashTimer->reset();
            dashCount--;
        }
    }
}

void EyeOfCtulhu::switchPhase() {
    physics->accel = Vec2(0, 0);
    if(switchTimer->getTimeLeft() > switchTime / 2) {
        if(rotateSpeed < 20) rotateSpeed += 0.2;
    }
    else {
        if(rotateSpeed > 0.5) rotateSpeed -= 0.2;
        //setTexture(phase2);
    }
    transform->angle += rotateSpeed;
}

void EyeOfCtulhu::render() {
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(this->renderer);
    renderer->setState("idle");
}
