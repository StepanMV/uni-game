#include "enemy.h"

void Enemy::update() {
    physics->accel = Vec2(0, 0);
    skipPlatform = false;
    Vec2 direction = (target->pos - transform->pos);
    direction.y = 0;
    if(abs(direction.x) > 1) {
        direction.normalize();
        direction *= 1.5;
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
    collider->calcHitbox();
    onGround = false;
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

EnemyBuilder EnemyBuilder::spawn(unsigned id, Vec2 pos, Vec2 size) {
    EnemyBuilder builder;
    builder.enemy = std::shared_ptr<Enemy>(new Enemy());
    builder.enemy->id = id;
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