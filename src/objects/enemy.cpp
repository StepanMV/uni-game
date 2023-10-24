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
        case EnemyType::EYE: {
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
            //if(other->getDamage()) takeKnockback(other->getCenterOffset().x + other->getPos().x);
            damageTimer->reset();
        }
    }
}

std::shared_ptr<Enemy> EnemyBuilder::spawn(EnemyType type, Vec2 pos, std::shared_ptr<MyTransform> target) {
    EnemyBuilder builder;
    //auto renderer = std::dynamic_pointer_cast<CoolRenderer>(builder.enemy->renderer);

    IniFile ini("enemies.ini");

    switch(type) {
        case EnemyType::KingSlime: {
            builder.enemy = std::shared_ptr<Enemy>(new KingSlime());
            auto renderer = std::dynamic_pointer_cast<CoolRenderer>(builder.enemy->renderer);
            Vec2 textureSize = renderer->loadTexture("KingSlime", "resources/textures/KingSlime.png");
            renderer->addToState("idle", "KingSlime", TextureDataBuilder::init(TextureType::ANIMATION, "KingSlime", textureSize)
            .animation({1, 6}, {0, 0}, {0, 4}, 5).build());

            renderer->addToState("jump", "KingSlime", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "KingSlime", textureSize)
            .spriteSheet({1, 6}, {0, 5}).build());
            renderer->setState("idle");

            builder.enemy->contactDamage = ini.readInt("KingSlime", "contactDamage");
            builder.enemy->transform->size = Vec2(ini.readInt("KingSlime", "width"), ini.readInt("KingSlime", "height"));
            builder.enemy->tileCollide = ini.readBool("KingSlime", "tileCollide");
            builder.enemy->physics->maxMoveSpeed = ini.readDouble("KingSlime", "maxMoveSpeed");
            builder.enemy->physics->maxFallSpeed = ini.readDouble("KingSlime", "maxFallSpeed");
            builder.enemy->physics->maxFlySpeed = ini.readDouble("KingSlime", "maxFlySpeed");
            builder.enemy->physics->friction = ini.readDouble("KingSlime", "friction");
            builder.enemy->physics->gravity = ini.readDouble("KingSlime", "gravity");
        }
    }
    builder.enemy->id = 1;
    builder.enemy->transform->pos = pos;
    builder.enemy->collider = std::make_shared<Collider>(builder.enemy->transform);
    builder.enemy->target = target;
    Object::objects.push_back(builder.enemy);
    return builder.enemy;
}

// EnemyBuilder EnemyBuilder::spawn(unsigned id, EnemyType type, Vec2 pos, Vec2 size) {
//     EnemyBuilder builder;
//     builder.enemy = std::shared_ptr<Enemy>(new Enemy());
//     builder.enemy->id = id;
//     builder.enemy->type = type;
//     builder.enemy->transform->pos = pos;
//     builder.enemy->transform->size = size;
//     builder.enemy->tileCollide = true;
//     builder.enemy->collider = std::make_shared<Collider>(builder.enemy->transform);
//     return builder;
// }

EnemyBuilder& EnemyBuilder::setMaxSpeeds(double maxMoveSpeed, double maxFallSpeed, double maxFlySpeed) {
    enemy->physics->maxMoveSpeed = maxMoveSpeed;
    enemy->physics->maxFallSpeed = maxFallSpeed;
    enemy->physics->maxFlySpeed = maxFlySpeed;
    return *this;
}

EnemyBuilder& EnemyBuilder::setForces(double friction, double gravity) {
    enemy->physics->friction = friction;
    enemy->physics->gravity = gravity;
    return *this;
}

EnemyBuilder& EnemyBuilder::setTarget(std::shared_ptr<MyTransform> target) {
    enemy->target = target;
    return *this;
}

std::shared_ptr<Enemy> EnemyBuilder::build() {
    if(enemy->id == 0) return enemy;

    Object::objects.push_back(enemy);

    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(enemy->renderer);

    Vec2 textureSize = renderer->loadTexture("enemy", "resources/textures/Enemy_" + std::to_string(enemy->id) + ".png");

    renderer->addToState("idle", "enemy", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "enemy", textureSize)
    .spriteSheet({1, 3}, {0, 0}).build());
    
    renderer->addToState("move", "enemy", TextureDataBuilder::init(TextureType::ANIMATION, "enemy", textureSize)
    .animation({1, 3}, {0, 0}, {0, 2}, 2).build());

    renderer->addToState("jump", "enemy", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "enemy", textureSize)
    .spriteSheet({1, 3}, {0, 1}).build());

    renderer->setState("idle");
    return enemy;
}