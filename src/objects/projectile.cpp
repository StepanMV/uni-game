#include "projectile.h"
#include <cmath>
#include "tile.h"

Projectile::Projectile(unsigned _id, unsigned _damage, const bool _fromPlayer): damage(_damage), fromPlayer(_fromPlayer),
id(_id) {
    if(id == 1) {
        renderer.loadTexture("star", "resources/textures/Gore_16.png");
        renderer.addToState("fly", "star").spriteSheet({1, 1}, {0, 0});
        physics.friction = 0;
        physics.gravity = 0.1;
    }
}

void Projectile::breakProjectile() {
    id = 0;
}

bool Projectile::isAlive() const {
    return id != 0;
}

void Projectile::spawn(Vec2 pos, Vec2 size, double lifetime) {
    timer = std::move(Timer(lifetime, [this](){this->breakProjectile();}));//как?
    renderer.setState("fly");
    this->pos = pos;
    this->size = size;
}

void Projectile::setDirection(Vec2 target) {
    physics.speed = target - pos;
    calcHitbox();
    physics.speed.normalize();
    physics.speed *= 5;
}

void Projectile::onCollision(Tile& other) {
    if(other.getId() == 1) {
        breakProjectile();
    }
}

unsigned Projectile::getId() const {
    return id;
}

void Projectile::update() {
    angle = atan2(physics.speed.y , physics.speed.x) * 180 / M_PI;
    calcHitbox();
    if(timer.isDone()) {
        breakProjectile();
    }
    timer.update();
}

void Projectile::render() {
    renderer.setRotation(angle);
    renderer.update(pos, size);
    renderer.render();
}