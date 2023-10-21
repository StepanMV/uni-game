#include "projectile.h"
#include <cmath>
#include "tile.h"
#include <iostream>

bool Projectile::isCollideable() const {
    return id != 0;
}

void Projectile::setDirection(Vec2 target)
{
    physics->speed = target - transform->pos;
    collider->calcHitbox();
    physics->speed.normalize();
    physics->speed *= 10;
}

void Projectile::onCollision(std::shared_ptr<Tile> other) {
    if(!other->isPlatform) {
        destroy();
    }
}

void Projectile::onCollision(std::shared_ptr<Enemy> other) {
    if(fromPlayer) destroy();
}

void Projectile::onCollision(std::shared_ptr<Projectile> other) {
    
}

void Projectile::onCollision(std::shared_ptr<Player> other) {
    if(!fromPlayer) destroy();
}

void Projectile::setId(unsigned id) {
    this->id = id;
}

void Projectile::update() {
    transform->angle = atan2(physics->speed.y , physics->speed.x) * 180 / M_PI;
    collider->calcHitbox();
    if(timer->isDone()) {
        destroy();
    }
}  

void Projectile::render() {
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(this->renderer);
    renderer->setRotation(transform->angle);
}

ProjectileBuilder ProjectileBuilder::spawn(Vec2 pos, Vec2 size, unsigned _id) {
    ProjectileBuilder builder;
    builder.projectile = std::shared_ptr<Projectile>(new Projectile());
    builder.projectile->transform->pos = pos;
    builder.projectile->transform->size = size;
    builder.projectile->id = _id;
    builder.projectile->renderer = std::make_shared<CoolRenderer>(builder.projectile->transform);
    builder.projectile->physics = std::make_shared<Physics>();
    return builder;
}

ProjectileBuilder &ProjectileBuilder::extra(double lifetime, unsigned _damage, bool _fromPlayer) {
    projectile->timer = Timer::getInstance(lifetime);
    projectile->damage = _damage;
    projectile->fromPlayer = _fromPlayer;
    return *this;
}

std::shared_ptr<Projectile> ProjectileBuilder::build() {
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(projectile->renderer);
    if(projectile->id == 1) {
        renderer->loadTexture("star", "resources/textures/Gore_16.png");
        Vec2 textureSize = renderer->getTextureSize("star");
        renderer->addToState("fly", "star", TextureDataBuilder::init(TextureType::TEXTURE, "star", textureSize).build());
        renderer->setState("fly");
        projectile->physics->friction = 0;
        projectile->physics->gravity = 0;
        projectile->physics->maxFallSpeed = 0;
        projectile->physics->maxFlySpeed = 100;
    }
    Object::objects.push_back(projectile);
    return projectile;
}
