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
    centerOffset = other.centerOffset;
    startCenter = other.startCenter;
    startHitbox = other.startHitbox;
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
    centerOffset = other.centerOffset;
    startCenter = other.startCenter;
    startHitbox = other.startHitbox;
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
        physics->gravity = 0;//0.1
        physics->maxMoveSpeed =10;
        physics->maxFallSpeed =10;
        physics->maxFlySpeed = 10;
    }
    else if(id == 2) {
        renderer->loadTexture("sword", "resources/textures/sword.png");
        Vec2 textureSize = renderer->getTextureSize("sword");
        renderer->addToState("attack", "sword", TextureDataBuilder::init(TextureType::TEXTURE, "sword", textureSize).build());
        physics->friction = 0;
        physics->gravity = 0;
    }
}

void Projectile::breakProjectile() {
    id = 0;
}

bool Projectile::isAlive() const {
    return id != 0 && id != 2;
}

void Projectile::spawn(Vec2 pos, Vec2 size, double lifetime) {
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(this->renderer);
    if(lifetime > 0) {
        timer = Timer::getInstance(lifetime);
    }
    else {
        timer = std::make_shared<Timer>();
    }
    if(id == 1) renderer->setState("fly");
    if(id == 2) renderer->setState("attack");
    this->pos = pos;
    this->size = size;
}

void Projectile::setDirection(Vec2 target) {
    physics->speed = target - pos;
    calcHitbox();
    physics->speed.normalize();
    physics->speed *= 5;
}

void Projectile::setAccelDirection(Vec2 target) {
    physics->accel = target - pos;
    physics->speed *= 0.99;
    calcHitbox();
    physics->accel.normalize();
    //physics->accel *= 5;
}

void Projectile::onCollision(Tile& other) {
    if(other.getId() != 0) {
        breakProjectile();
    }
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
    if(id == 1) {
        angle = atan2(physics->speed.y , physics->speed.x) * 180 / M_PI;
    }
    if(id == 2) {
        angle += 10;
        if(angle > 135) {
            breakProjectile();
            angle = -30;
        }
        angle = (int)angle % 360;
    }
    calcHitbox();
    if(timer->isDone()) {
        breakProjectile();
    }
}   

void Projectile::render() {
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(this->renderer);
    renderer->setRotation(angle);
    renderer->render();
}