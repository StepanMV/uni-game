#include "enemy.h"
#include "projectile.h"

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
        case EnemyType::SLIME: {
            physics->accel.y = 0;
            if(onGround) {
                if(!stayTimer->isDone()) {
                    physics->accel.x = 0;
                    break;
                }
                else {
                    stayTimer->reset();
                }
                Vec2 direction = (target->pos - transform->pos);
                direction.y = 0;
                direction.normalize();
                direction *= 1.5;
                move(direction);
            }
            jump();
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

EnemyBuilder EnemyBuilder::spawn(unsigned id, EnemyType type, Vec2 pos, Vec2 size) {
    EnemyBuilder builder;
    builder.enemy = std::shared_ptr<Enemy>(new Enemy());
    builder.enemy->id = id;
    builder.enemy->type = type;
    builder.enemy->transform->pos = pos;
    builder.enemy->transform->size = size;
    builder.enemy->tileCollide = true;
    builder.enemy->collider = std::make_shared<Collider>(builder.enemy->transform);
    return builder;
}

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