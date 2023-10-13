#include "projectile.h"
#include <cmath>
#include "tile.h"
#include <iostream>

Projectile::Projectile() {
    renderer = std::make_shared<CoolRenderer>();
    physics = std::make_shared<Physics>();
}

Projectile::Projectile(const Projectile &other) {
    renderer = other.renderer;
    physics = other.physics;
    pos = other.pos;
    size = other.size;
    hitbox = other.hitbox;
    angle = other.angle;
    timer = other.timer;
    damage = other.damage;
    fromPlayer = other.fromPlayer;
    id = other.id;
    renderer->changeObject(&pos);
}

Projectile &Projectile::operator=(const Projectile &other) {
    renderer = other.renderer;
    physics = other.physics;
    pos = other.pos;
    size = other.size;
    hitbox = other.hitbox;
    angle = other.angle;
    timer = other.timer;
    damage = other.damage;
    fromPlayer = other.fromPlayer;
    id = other.id;
    renderer->changeObject(&pos);
    return *this;
}

Projectile::Projectile(unsigned _id, unsigned _damage, const bool _fromPlayer) : damage(_damage), fromPlayer(_fromPlayer),
                                                                                 id(_id)
{
    renderer = std::make_shared<CoolRenderer>(&pos);
    physics = std::make_shared<Physics>();
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(this->renderer);
    if(id == 1) {
        renderer->loadTexture("star", "resources/textures/Gore_16.png");
        Vec2 textureSize = renderer->getTextureSize("star");
        renderer->addToState("fly", "star", TextureDataBuilder::init(TextureType::TEXTURE, "star", textureSize).build());
        physics->friction = 0;
        physics->gravity = 0;
    }
}

void Projectile::breakObject() {
    id = 0;
}

bool Projectile::isAlive() const {
    return id != 0;
}

bool Projectile::isCollideable() const {
    return id != 0;
}

void Projectile::spawn(Vec2 pos, Vec2 size, double lifetime) {
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(this->renderer);
    timer = Timer::getInstance(lifetime);
    renderer->setState("fly");
    this->pos = pos;
    this->size = size;
}

void Projectile::setDirection(Vec2 target)
{
    physics->speed = target - pos;
    calcHitbox();
    physics->speed.normalize();
    physics->speed *= 5;
}

void Projectile::onCollision(std::shared_ptr<Tile> other) {
    if(other->getId() != 0) {
        breakObject();
    }
}

void Projectile::onCollision(std::shared_ptr<Entity> other) {

}

void Projectile::onCollision(std::shared_ptr<Projectile> other) {
    
}

unsigned Projectile::getId() const {
    return id;
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