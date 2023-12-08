#include "projectile.h"
#include <cmath>
#include "tile.h"
#include <iostream>
#include "ini_file.h"
#include "audio.h"
#include "particle.h"

std::shared_ptr<Projectile> Projectile::spawn(unsigned id, Vec2 pos, bool fromPlayer, unsigned weaponDamage) {
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

    proj->particles = ini.readUInt(strId, "particles");
    proj->damage = ini.readInt(strId, "damage") + weaponDamage;
    proj->timer = Timer::getInstance(ini.readDouble(strId, "lifetime"));
    proj->hitCount = ini.readInt(strId, "hitcount");
    proj->destroySound = ini.readString(strId, "destroySound");
    Audio::playSound(ini.readString(strId, "spawnSound"), 0.25);
    proj->physics->friction = ini.readDouble(strId, "friction");
    proj->physics->gravity = ini.readDouble(strId, "gravity");
    proj->physics->maxMoveSpeed = ini.readDouble(strId, "maxMoveSpeed");
    proj->physics->maxFallSpeed = proj->physics->maxFlySpeed = proj->physics->maxMoveSpeed;
    proj->transform->size = Vec2(ini.readInt(strId, "width"), ini.readInt(strId, "height"));
    proj->hitTimer->stop();

    Object::addProjectile(proj);
    return proj;
}

bool Projectile::isCollideable() const {
    return id != 0;
}

void Projectile::setDirection(Vec2 direction) {
    physics->speed = direction;
    physics->speed.normalize();
    physics->speed *= physics->maxMoveSpeed;
}

void Projectile::onCollision(std::shared_ptr<Tile> other) {
    if(!other->isPlatform) {
        Audio::playSound(destroySound, 0.5);
        destroy();
    }
}

void Projectile::onCollision(std::shared_ptr<Enemy> other) {
    if(fromPlayer) {
        if(hitTimer->isDone()) {
            hitCount--;
            hitTimer->reset();
        }
        if(hitCount <= 0) destroy();
    }
}

void Projectile::onCollision(std::shared_ptr<Player> other) {
    if(!fromPlayer) {
        if(hitTimer->isDone()) {
            hitCount--;
            hitTimer->reset();
        }
        if(hitCount <= 0) destroy();
    }
}

void Projectile::setId(unsigned id) {
    this->id = id;
}

void Projectile::update() {
    if(particles) {
        ParticleBuilder::init("Particle_1", transform->pos, 0.5, GetColor(particles))
            .setAmount(1, 1.0f * transform->size)
            .setFadeTime(0.5, 100, true)
            .setPhys(Vec2(50, 50), 0.1, 0)
            .build();
    }
    transform->angle = atan2(physics->speed.y , physics->speed.x) * 180 / 3.14;
    collider->calcHitbox();
    if(timer->isDone()) {
        destroy();
    }
}  

bool Projectile::getFromPlayer() const {
    return fromPlayer;
}

unsigned Projectile::getDamage() const {
    if(hitTimer->isDone()) {
        return damage;
    }
    else {
        return 0;
    }
}
