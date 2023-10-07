#include "projectile.h"

Projectile::Projectile(unsigned _damage, const bool _fromPlayer): damage(_damage), fromPlayer(_fromPlayer),
id(1) {
    if(id == 1) {
        size = Vec2(22, 24);
        renderer.loadTexture("star", "resources/textures/Armor_Head_3.png");
        renderer.addToState("fly", "star");
    }
}

void Projectile::spawn(Vec2 pos, Vec2 size) {
    renderer.setState("fly");
    this->pos = pos;
    this->size = size;
}

void Projectile::setSpeed(Vec2 speed) {
    physics.speed = speed;
}

void Projectile::setSpeed(Vec2 start, Vec2 target) {
    physics.speed = target - start;
    //physics.speed = physics.speed.normalize() * 10;
}

void Projectile::update() {
    
}

void Projectile::render() {
    renderer.setState("fly");
    renderer.update(pos, size);
    renderer.render();
}