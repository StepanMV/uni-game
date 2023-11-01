#include "projectile.h"
#include <cmath>
#include "tile.h"
#include <iostream>
#include "ini_file.h"
#include "audio.h"

std::shared_ptr<Projectile> Projectile::spawn(unsigned id, Vec2 pos, bool fromPlayer) {
    std::shared_ptr<Projectile> proj = std::shared_ptr<Projectile>(new Projectile());
    proj->renderer = std::make_shared<CoolRenderer>(proj->transform);
    proj->physics = std::make_shared<Physics>();
    proj->collider = std::make_shared<Collider>(proj->transform);
    proj->transform->pos = pos;
    proj->id = id;
    proj->fromPlayer = fromPlayer;

    std::string strId = "Projectile_" + std::to_string(id);

    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(proj->renderer);
    Vec2 textureSize = renderer->loadTexture(strId, "resources/textures/" + strId + ".png");
    renderer->addToState("fly", strId, TextureDataBuilder::init(TextureType::TEXTURE, strId, textureSize).build());
    renderer->setState("fly");

    IniFile ini("projectiles.ini");

    proj->damage = ini.readInt(strId, "damage");
    proj->timer = Timer::getInstance(ini.readDouble(strId, "lifetime"));
    proj->destroySound = ini.readString(strId, "destroySound");
    proj->spawnSound = ini.readString(strId, "spawnSound");
    Audio::playSound(proj->spawnSound, 0.25);
    proj->physics->friction = ini.readDouble(strId, "friction");
    proj->physics->gravity = ini.readDouble(strId, "gravity");
    proj->physics->maxMoveSpeed = ini.readDouble(strId, "maxMoveSpeed");
    proj->physics->maxFallSpeed = proj->physics->maxFlySpeed = proj->physics->maxMoveSpeed;
    proj->transform->size = Vec2(ini.readInt(strId, "width"), ini.readInt(strId, "height"));

    Object::addProjectile(proj);
    return proj;
}

bool Projectile::isCollideable() const {
    return id != 0;
}

void Projectile::setDirection(Vec2 target) {
    physics->speed = target - transform->pos;
    collider->calcHitbox();
    physics->speed.normalize();
    physics->speed *= 10;
}

void Projectile::onCollision(std::shared_ptr<Tile> other) {
    if(!other->isPlatform) {
        Audio::playSound(destroySound, 0.5);
        destroy();
    }
}

void Projectile::onCollision(std::shared_ptr<Enemy> other) {
    if(fromPlayer) destroy();
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

bool Projectile::getFromPlayer() const {
    return fromPlayer;
}

unsigned Projectile::getDamage() const {
    return damage;
}
