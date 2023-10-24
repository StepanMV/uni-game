#include "entity.h"
#include "tile.h"

bool Entity::getAttacking() const {
    return isAttacking;
}

Entity::Entity() {
    this->renderer = std::make_shared<CoolRenderer>(transform);
    this->physics = std::make_shared<Physics>();
    this->collider = std::make_shared<Collider>(transform);
}

void Entity::onBoard() {
    if(transform->pos.x - transform->size.x / 2 < Level::borderOffset * Level::tileSize) {
        physics->speed.x = 0;
        transform->pos.x = Level::borderOffset * Level::tileSize + transform->size.x / 2;
    }
    if(transform->pos.y - transform->size.y / 2 < Level::borderOffset * Level::tileSize) {
        physics->speed.y = 0;
        transform->pos.y = Level::borderOffset * Level::tileSize + transform->size.y / 2;
    }
    if(transform->pos.x + transform->size.x / 2 > (Level::width - Level::borderOffset) * Level::tileSize) {
        physics->speed.x = 0;
        transform->pos.x = (Level::width - Level::borderOffset) * Level::tileSize - transform->size.x / 2;
    }
    if(transform->pos.y + transform->size.y / 2 > (Level::height - Level::borderOffset) * Level::tileSize)
    {
        physics->speed.y = 0;
        transform->pos.y = (Level::height - Level::borderOffset) * Level::tileSize - transform->size.y / 2;
        onGround = true;
    }
}

void Entity::move(Vec2 direction) {
    physics->accel += direction;
}

void Entity::dash(Vec2 direction, double dashSpeed) {
    physics->speed = direction;
    physics->speed.normalize();
    physics->speed *= dashSpeed;
}

void Entity::plane() {
    if(!onGround && !currentFlightTime) {
        if(physics->speed.y > physics->maxFallSpeed / 5) {
            physics->speed.y = physics->maxFallSpeed / 5;
        }
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
    if(!isFlying && !onGround) {
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
    if(onGround) {
        jumpTimer->reset();
        physics->speed.y += -20;
        onGround = false;
    }
    else {
        if(!jumpTimer->isDone()) {
            physics->accel += Vec2(0, -1.5);
        }
    }
}

void Entity::takeDamage(unsigned damage) {
    health -= damage;
    if(health <= 0) {
        this->destroy();
    }
}

void Entity::takeKnockback(float projPosX) {
    Vec2 knockback(1, 0);
    if(projPosX - transform->pos.x >= 0) {
        knockback.rotate(20);
    }
    else {
        knockback.rotate(160);
    }
    knockback *= 20;
    physics->accel = Vec2(0, 0);
    physics->speed = -knockback;
}

void Entity::onCollision(std::shared_ptr<Tile> other) {
    if(other->isPlatform && skipPlatform) {
        return;
    }
    if(!other->isUp && (physics->speed.y > 0) && (transform->pos.y + transform->size.y / 2 < other->getPos().y + other->getSize().y / 2)) {
        if(other->getPos().x + other->getSize().x / 2 - transform->pos.x + transform->size.x / 2 <= 2) {
            transform->pos.x = other->getPos().x + other->getSize().x / 2 + transform->size.x / 2 - 1;
        }
        else if(transform->pos.x + transform->size.x / 2 - other->getPos().x + other->getSize().x / 2 <= 2) {
            transform->pos.x = other->getPos().x - other->getSize().x / 2 - transform->size.x / 2 + 1;
        }
        else {
            physics->speed.y = 0;
            onGround = true;
            transform->pos.y = other->getPos().y - other->getSize().y / 2 - transform->size.y / 2 + 1;
        }
        collider->calcHitbox();
    }
    if(other->isPlatform) return;
    if(!other->isDown && physics->speed.y < 0 && transform->pos.y - transform->size.y / 2 > other->getPos().y - other->getSize().y / 2){
        if(other->getPos().x + other->getSize().x / 2 - transform->pos.x + transform->size.x / 2 <= 2) {
            transform->pos.x = other->getPos().x + other->getSize().x / 2 + transform->size.x / 2 - 1;
        }
        else if(transform->pos.x + transform->size.x / 2 - other->getPos().x + other->getSize().x / 2 <= 2) {
            transform->pos.x = other->getPos().x - other->getSize().x / 2 - transform->size.x / 2 + 1;
        }
        else {
            jumpTimer->stop();
            physics->speed.y = 0;
            transform->pos.y = other->getPos().y + other->getSize().y / 2 + transform->size.y / 2 - 1;
        }
        collider->calcHitbox();
    }
    if(!other->isLeft && (physics->speed.x > 0) && (transform->pos.x + transform->size.x / 2 < other->getPos().x + other->getSize().x / 2)) {
        if((other->canClimbLeft) && (transform->pos.y <= other->getPos().y - other->getSize().y / 2)) {
            transform->pos.y = other->getPos().y - other->getSize().y / 2 - transform->size.y / 2 + 1;
        }
        else if(other->getPos().y + other->getSize().y / 2 - transform->pos.y + transform->size.y / 2 <= 2) {
            transform->pos.y = other->getPos().y + other->getSize().y / 2 + transform->size.y / 2 - 1;
        }
        else {
            physics->speed.x = 0;
            transform->pos.x = other->getPos().x - other->getSize().x / 2 - transform->size.x / 2 + 1;
        }
        collider->calcHitbox();
    }
    if(!other->isRight && (physics->speed.x < 0) && (transform->pos.x - transform->size.x / 2 > other->getPos().x - other->getSize().x / 2)) {
        if((other->canClimbRight) && (transform->pos.y <= other->getPos().y - other->getSize().y / 2)) {
            transform->pos.y = other->getPos().y - other->getSize().y / 2 - transform->size.y / 2 + 1;
        }
        else if(other->getPos().y + other->getSize().y / 2 - transform->pos.y + transform->size.y / 2 <= 2) {
            transform->pos.y = other->getPos().y + other->getSize().y / 2 + transform->size.y / 2 - 1;
        }
        else {
            physics->speed.x = 0;
            transform->pos.x = other->getPos().x + other->getSize().x / 2 + transform->size.x / 2 - 1;
        }
        collider->calcHitbox();
    }
}