#include "entity.h"

bool Entity::getAttacking() const {
    return isAttacking;
}

Entity::Entity() {
    this->renderer = std::make_shared<CoolRenderer>(transform);
    this->physics = std::make_shared<Physics>();
    this->collider = std::make_shared<Collider>(transform);
}

void Entity::move(Vec2 direction) {
    physics->accel += direction;
}

void Entity::dash(Vec2 direction) {
    physics->speed = direction;
    physics->speed.normalize();
    physics->speed *= 30;
}

void Entity::plane() {
    if(!physics->onGround && !currentFlightTime) {
        physics->speed.y = physics->maxFallSpeed / 5;
    }
}

void Entity::falling() {
    isFlying = false;
    if(!flyTimer->isDone()) {
        currentFlightTime = flyTimer->getTimeLeft();
    }
    flyTimer->stop();
}

void Entity::fly() {
    if(!isFlying && !physics->onGround) {
        isFlying = true;
        flyTimer = Timer::getInstance(currentFlightTime);
    }
    if(!flyTimer->isDone()) {
        physics->accel += Vec2(0, -1.5);
    }
    else {
        currentFlightTime = 0;
    }
}

void Entity::jump() {
    if(physics->onGround) {
        physics->speed.y += -20;
        physics->onGround = false;
    }
}