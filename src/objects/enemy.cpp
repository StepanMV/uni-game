#include "enemy.h"
#include "projectile.h"
#include "ini_file.h"
#include "bosses.h"
#include "audio.h"

unsigned Enemy::getContactDamage() const {
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
    damageSound = ini.readString(enemyName, "damageSound");
    destroySound = ini.readString(enemyName, "destroySound");
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