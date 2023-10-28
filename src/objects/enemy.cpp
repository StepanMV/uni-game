#include "enemy.h"
#include "projectile.h"
#include "ini_file.h"
#include "bosses.h"

void Enemy::update() {
    switch(type) {
        case EnemyType::ZOMBIE: {
            physics->accel = Vec2(0, 0);
            skipPlatform = false;
            Vec2 direction = (target->pos - transform->pos);
            direction.y = 0;
            if(abs(direction.x) > 1) {
                direction.normalize();
                direction *= 0.75;
                move(direction);
            }
            else {
                if(target->pos.y < transform->pos.y) {
                    jump();
                }
            }
            if(target->pos.y > transform->pos.y) {
                skipPlatform = true;
            }
            break;
        }
        case EnemyType::Eye: {
            tileCollide = false;
            Vec2 direction = (target->pos - transform->pos);
            if(abs(direction.x) < 5) {
                physics->speed *= 0;
            }
            else {
                physics->speed *= 0.1;
                transform->angle = atan(direction.y / direction.x) * 180 / M_PI;
            }
            direction += Vec2(0, -target->size.y * 5);
            direction.normalize();
            move(direction);
            physics->accel *= 0.9;

            break;
        }
    }
    onGround = false;
    collider->calcHitbox();
}

void Enemy::render() {
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(this->renderer);
    renderer->setState("idle");
    if (physics->speed.x > 0) {
        facingLeft = false;
        renderer->setState("move");
        renderer->setAnimationSpeed(10 * physics->speed.x / physics->maxMoveSpeed);
    } else if (physics->speed.x < 0) {
        facingLeft = true;
        renderer->setState("move");
        renderer->setAnimationSpeed(10 * -physics->speed.x / physics->maxMoveSpeed);
    }
    if (!onGround) {
        renderer->setState("jump");
    }
    renderer->setFlipped(!facingLeft);
}

unsigned Enemy::getContactDamage() const
{
    return contactDamage;
}

bool Enemy::isCollideable() const {
    return true;
}

void Enemy::onCollision(std::shared_ptr<Projectile> other) {
    if(other->getFromPlayer()) {
        if(damageTimer->isDone()) {
            takeDamage(other->getDamage());
            if(other->getDamage()) takeKnockback(other->getCenterOffset().x + other->getPos().x);
            damageTimer->reset();
        }
    }
}

void Enemy::readStats(std::string enemyName) {
    IniFile ini("enemies.ini");
    max_health = ini.readInt(enemyName, "maxHealth");
    health = max_health;
    contactDamage = ini.readInt(enemyName, "contactDamage");
    transform->size = Vec2(ini.readInt(enemyName, "width"), ini.readInt(enemyName, "height"));
    tileCollide = ini.readBool(enemyName, "tileCollide");
    knockbackResist = ini.readBool(enemyName, "knockbackResist");
    physics->maxMoveSpeed = ini.readDouble(enemyName, "maxMoveSpeed");
    physics->maxFallSpeed = ini.readDouble(enemyName, "maxFallSpeed");
    physics->maxFlySpeed = ini.readDouble(enemyName, "maxFlySpeed");
    physics->friction = ini.readDouble(enemyName, "friction");
    physics->gravity = ini.readDouble(enemyName, "gravity");
    id = 1;
    collider = std::make_shared<Collider>(transform);
}