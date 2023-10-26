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

std::shared_ptr<Enemy> EnemyBuilder::spawn(EnemyType type, Vec2 pos, std::shared_ptr<MyTransform> target) {
    EnemyBuilder builder;
    //auto renderer = std::dynamic_pointer_cast<CoolRenderer>(builder.enemy->renderer);

    IniFile ini("enemies.ini");
    std::string enemyName;

    switch(type) {
        case EnemyType::KingSlime: {
            builder.enemy = std::shared_ptr<Enemy>(new KingSlime());
            enemyName = "KingSlime";
            auto renderer = std::dynamic_pointer_cast<CoolRenderer>(builder.enemy->renderer);
            Vec2 textureSize = renderer->loadTexture("KingSlime", "resources/textures/KingSlime.png");
            renderer->addToState("idle", "KingSlime", TextureDataBuilder::init(TextureType::ANIMATION, "KingSlime", textureSize)
            .animation({1, 6}, {0, 0}, {0, 4}, 5).build());

            renderer->addToState("jump", "KingSlime", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "KingSlime", textureSize)
            .spriteSheet({1, 6}, {0, 5}).build());
            renderer->setState("idle");
            break;
        }
        case EnemyType::Slime: {
            builder.enemy = std::shared_ptr<Enemy>(new Slime());
            enemyName = "Slime";
            auto renderer = std::dynamic_pointer_cast<CoolRenderer>(builder.enemy->renderer);
            Vec2 textureSize = renderer->loadTexture("Slime", "resources/textures/KingSlime.png");
            renderer->addToState("idle", "Slime", TextureDataBuilder::init(TextureType::ANIMATION, "Slime", textureSize)
            .animation({1, 6}, {0, 0}, {0, 4}, 5).build());

            renderer->addToState("jump", "Slime", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "Slime", textureSize)
            .spriteSheet({1, 6}, {0, 5}).build());
            renderer->setState("idle");

            builder.enemy->knockbackResist = false;
            break;
        }
        case EnemyType::EyeOfCtulhu: {
            builder.enemy = std::shared_ptr<Enemy>(new EyeOfCtulhu());
            enemyName = "EyeOfCtulhu";
            builder.enemy->tileCollide = false;
            auto renderer = std::dynamic_pointer_cast<CoolRenderer>(builder.enemy->renderer);
            Vec2 textureSize = renderer->loadTexture("EyeOfCtulhu", "resources/textures/KingSlime.png");
            renderer->addToState("idle", "EyeOfCtulhu", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "EyeOfCtulhu", textureSize)
            .spriteSheet({1, 6}, {0, 0}).build());
            break;
        }
        case EnemyType::Eye: {
            builder.enemy = std::shared_ptr<Enemy>(new Eye());
            enemyName = "Eye";
            builder.enemy->tileCollide = false;
            auto renderer = std::dynamic_pointer_cast<CoolRenderer>(builder.enemy->renderer);
            Vec2 textureSize = renderer->loadTexture("Eye", "resources/textures/KingSlime.png");
            renderer->addToState("idle", "Eye", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "Eye", textureSize)
            .spriteSheet({1, 6}, {0, 0}).build());
            break;
        }
    }
    builder.enemy->max_health = ini.readInt(enemyName, "maxHealth");
    builder.enemy->health = builder.enemy->max_health;
    builder.enemy->contactDamage = ini.readInt(enemyName, "contactDamage");
    builder.enemy->transform->size = Vec2(ini.readInt(enemyName, "width"), ini.readInt(enemyName, "height"));
    builder.enemy->tileCollide = ini.readBool(enemyName, "tileCollide");
    builder.enemy->physics->maxMoveSpeed = ini.readDouble(enemyName, "maxMoveSpeed");
    builder.enemy->physics->maxFallSpeed = ini.readDouble(enemyName, "maxFallSpeed");
    builder.enemy->physics->maxFlySpeed = ini.readDouble(enemyName, "maxFlySpeed");
    builder.enemy->physics->friction = ini.readDouble(enemyName, "friction");
    builder.enemy->physics->gravity = ini.readDouble(enemyName, "gravity");
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