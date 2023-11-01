#include "weapon.h"
#include "game.h"
#include "audio.h"

std::shared_ptr<Weapon> Weapon::spawn(WeaponType type, unsigned id, std::shared_ptr<MyTransform> refTransform, bool fromPlayer) {
    std::shared_ptr<Weapon> weapon = std::shared_ptr<Weapon>(new Weapon());
    weapon->renderer = std::make_shared<CoolRenderer>(weapon->transform);
    weapon->tileCollide = false;
    weapon->physics = std::make_shared<Physics>();
    weapon->collider = std::make_shared<Collider>(weapon->transform);

    weapon->refTransform = refTransform;
    weapon->id = id;
    weapon->fromPlayer = fromPlayer;
    weapon->type = type;

    std::string strId = "";
    if(type == WeaponType::SWORD) {
        strId = "Sword_";
    }
    else if(type == WeaponType::GUN) {
        strId = "Gun_";
    }
    strId += std::to_string(id);

    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(weapon->renderer);
    Vec2 textureSize = renderer->loadTexture(strId, "resources/textures/" + strId + ".png");
    renderer->addToState("attack", strId, TextureDataBuilder::init(TextureType::TEXTURE, strId, textureSize).build());
    renderer->setState("attack");

    IniFile ini("weapons.ini");

    weapon->damage = ini.readInt(strId, "damage");
    weapon->timer = Timer::getInstance(ini.readDouble(strId, "attacktime"));
    weapon->attackSpeed = 180 / (ini.readDouble(strId, "attacktime") * 60);
    weapon->destroySound = ini.readString(strId, "destroySound");
    weapon->spawnSound = ini.readString(strId, "spawnSound");
    Audio::playSound(weapon->spawnSound);
    weapon->physics->friction = 0;
    weapon->physics->gravity = 0;
    weapon->physics->maxMoveSpeed = 0;
    weapon->physics->maxFallSpeed = weapon->physics->maxFlySpeed = weapon->physics->maxMoveSpeed;
    weapon->transform->size = Vec2(ini.readInt(strId, "width"), ini.readInt(strId, "height"));

    weapon->collider->setCenterOffset(Vec2(0, refTransform->size.y / 2 + weapon->transform->size.y / 2));
    weapon->collider->setPos(Vec2(refTransform->pos.x, refTransform->pos.y - refTransform->size.y / 2 - weapon->transform->size.y / 2));
    weapon->collider->calcHitbox();

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
            leftSide = transform->angle < 0 || transform->angle > 180;
            break;
        }
    }
    if(timer->isDone()) {
        destroy();
    }
    this->collider->setPos(Vec2(refTransform->pos.x, refTransform->pos.y - refTransform->size.y / 2 - transform->size.y / 2));
    collider->calcHitbox();
}

void Weapon::render() {
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(this->renderer);
    renderer->setFlipped(leftSide);
}

void Weapon::setLeftSide(bool _leftSide) {
    if(_leftSide != leftSide) {
        transform->angle = -transform->angle;
        attackSpeed = -attackSpeed;
    }
    leftSide = _leftSide;
}