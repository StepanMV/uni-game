#include "projectile.h"
#include <cmath>
#include "tile.h"
#include <iostream>

void Projectile::breakObject() {
    id = 0;
}

bool Projectile::isAlive() const {
    return id != 0;
}

bool Projectile::isCollideable() const {
    return id != 0;
}

void Projectile::setDirection(Vec2 target)
{
    physics->speed = target - pos;
    calcHitbox();
    physics->speed.normalize();
    physics->speed *= 5;
}

void Projectile::onCollision(std::shared_ptr<Tile> other) {
    if(!other->isPlatform) {
        breakObject();
    }
}

void Projectile::onCollision(std::shared_ptr<Enemy> other) {
    if(fromPlayer) breakObject();
}

void Projectile::onCollision(std::shared_ptr<Projectile> other) {
    
}

void Projectile::onCollision(std::shared_ptr<Player> other) {
    if(!fromPlayer) breakObject();
}

unsigned Projectile::getId() const {
    return id;
}

void Projectile::setId(unsigned id) {
    this->id = id;
}

void Projectile::setPos(Vec2 pos) {
    this->pos = pos;
    this->pos += startCenter;
}

void Projectile::update() {
    angle = atan2(physics->speed.y , physics->speed.x) * 180 / M_PI;
    calcHitbox();
    if(timer->isDone()) {
        breakObject();
    }
}  

void Projectile::render() {
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(this->renderer);
    renderer->setRotation(angle);
    renderer->render();
}

ProjectileBuilder ProjectileBuilder::spawn(Vec2 pos, Vec2 size, unsigned _id) {
    ProjectileBuilder builder;
    builder.projectile = std::make_shared<Projectile>();
    builder.projectile->pos = pos;
    builder.projectile->size = size;
    builder.projectile->id = _id;
    builder.projectile->renderer = std::make_shared<CoolRenderer>(&builder.projectile->pos);
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
    }
    Level::addObject(projectile);
    return projectile;
}
