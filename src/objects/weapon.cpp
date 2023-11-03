#include "weapon.h"
#include "game.h"
#include "audio.h"

std::shared_ptr<Weapon> Weapon::spawn(unsigned id, std::shared_ptr<MyTransform> refTransform, bool fromPlayer) {
    std::shared_ptr<Weapon> weapon = std::shared_ptr<Weapon>(new Weapon());
    weapon->renderer = std::make_shared<CoolRenderer>(weapon->transform);
    weapon->tileCollide = false;
    weapon->physics = std::make_shared<Physics>();
    weapon->collider = std::make_shared<Collider>(weapon->transform);

    weapon->refTransform = refTransform;
    weapon->id = id;
    weapon->fromPlayer = fromPlayer;
    std::string strId = "Weapon_" + std::to_string(id);

    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(weapon->renderer);
    Vec2 textureSize = renderer->loadTexture(strId, "resources/textures/" + strId + ".png");
    renderer->addToState("attack", strId, TextureDataBuilder::init(TextureType::TEXTURE, strId, textureSize).build());
    renderer->setState("attack");

    IniFile ini("weapons.ini");

    std::string strType = ini.readString(strId, "type");
    if(strType == "Sword") {
        weapon->type = WeaponType::SWORD;
        weapon->transform->angle = -45;
    }
    else if(strType == "Gun") {
        weapon->type = WeaponType::GUN;
        weapon->objectCollide = false;
    }
    weapon->damage = ini.readInt(strId, "damage");
    weapon->timer = Timer::getInstance(ini.readDouble(strId, "attacktime"));
    weapon->attackSpeed = 180 / (ini.readDouble(strId, "attacktime") * 60);
    weapon->destroySound = ini.readString(strId, "destroySound");
    Audio::playSound(ini.readString(strId, "spawnSound"));
    weapon->physics->friction = 0;
    weapon->physics->gravity = 0;
    weapon->physics->maxMoveSpeed = 0;
    weapon->physics->maxFallSpeed = weapon->physics->maxFlySpeed = weapon->physics->maxMoveSpeed;
    weapon->transform->size = Vec2(ini.readInt(strId, "width"), ini.readInt(strId, "height"));
    weapon->hitTimer = Timer::getInstance(0.5);
    weapon->hitTimer->stop();

    weapon->collider->setCenterOffset(Vec2(0, refTransform->size.y / 4 + weapon->transform->size.y / 2));
    weapon->collider->setPos(Vec2(refTransform->pos.x, refTransform->pos.y - refTransform->size.y / 4 - weapon->transform->size.y / 2));
    weapon->collider->calcHitbox();

    unsigned projDamage = ini.readInt(strId, "projdamage");
    std::string projType = ini.readString(strId, "projtype", "Default");
    unsigned projId = ini.readInt(strId, "projid");
    Vector2 mousePos = GetScreenToWorld2D({(float) GetMouseX(), (float) GetMouseY()}, Level::camera->getCamera());
    Vec2 worldMP = Vec2(mousePos.x, mousePos.y);
    Vec2 spawnPos = worldMP - refTransform->pos;
    spawnPos.normalize();
    spawnPos *= (refTransform->size.x / 2 + weapon->getSize().y / 2);
    spawnPos += refTransform->pos;
    Vec2 projDirection = worldMP - refTransform->pos;
    if(projType == "Default") {
        auto proj = Projectile::spawn(projId, spawnPos, weapon->fromPlayer, projDamage);
        proj->setDirection(projDirection);
    }
    else if(projType == "Shotgun") {
        int projCount = GetRandomValue(4, 7);
        for(int i = 0; i < projCount; i++) {
            auto proj = Projectile::spawn(projId, spawnPos, weapon->fromPlayer, projDamage);
            Vec2 randomProjDirection = projDirection;
            randomProjDirection.rotate(GetRandomValue(-15, 15));
            proj->setDirection(randomProjDirection);
        }
    }
    else if(projType == "SkyFalling") {
        auto proj = Projectile::spawn(projId, Vec2(worldMP.x + GetRandomValue(-100, 100), refTransform->pos.y - GetScreenHeight()), weapon->fromPlayer, projDamage);
        proj->setDirection(worldMP - proj->getPos());
    }

    Object::addProjectile(weapon);
    return weapon;
}

void Weapon::update() {
    switch(type) {
        case WeaponType::SWORD: {
            transform->angle += attackSpeed;
            break;
        }
        case WeaponType::GUN: {
            Vector2 mousePos = GetMousePosition();
            mousePos = GetScreenToWorld2D(mousePos, Level::camera->getCamera());
            transform->angle = atan2(mousePos.y - refTransform->pos.y, mousePos.x - refTransform->pos.x) * 180 / M_PI + 90;
            toLeft = transform->angle < 0 || transform->angle > 180;
            break;
        }
    }
    if(timer->isDone()) {
        destroy();
    }
    this->collider->setPos(Vec2(refTransform->pos.x, refTransform->pos.y - refTransform->size.y / 4 - transform->size.y / 2));
    collider->calcHitbox();
}

void Weapon::render() {
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(this->renderer);
    renderer->setFlipped(toLeft);
}

void Weapon::setLeftSide(bool _leftSide) {
    if(_leftSide != toLeft) {
        transform->angle = -transform->angle;
        attackSpeed = -attackSpeed;
    }
    toLeft = _leftSide;
}

void Weapon::onCollision(std::shared_ptr<Enemy> other) {
    if(fromPlayer) {
        if(hitTimer->isDone()) {
            hitTimer->reset();
        }
    }
}
