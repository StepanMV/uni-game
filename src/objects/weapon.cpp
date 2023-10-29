#include "weapon.h"
#include "game.h"

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

WeaponBuilder WeaponBuilder::spawn(std::shared_ptr<MyTransform> refTransform, Vec2 size, unsigned id) {
    WeaponBuilder builder;
    builder.weapon = std::shared_ptr<Weapon>(new Weapon());
    builder.weapon->refTransform = refTransform;
    builder.weapon->id = id;
    builder.weapon->transform->size = size;
    builder.weapon->renderer = std::make_shared<CoolRenderer>(builder.weapon->transform);
    builder.weapon->tileCollide = false;
    builder.weapon->physics = std::make_shared<Physics>();
    builder.weapon->collider = std::make_shared<Collider>(builder.weapon->transform);
    return builder;
}

WeaponBuilder& WeaponBuilder::extra(double _attackTime, unsigned _damage, WeaponType type, bool _fromPlayer) {
    weapon->type = type;
    if(weapon->type == WeaponType::SWORD) weapon->damage = _damage;
    else weapon->damage = 0;
    weapon->attackSpeed = 180 / (_attackTime * 60);
    weapon->timer = Timer::getInstance(_attackTime);
    weapon->fromPlayer = _fromPlayer;
    return *this;
}

std::shared_ptr<Weapon> WeaponBuilder::build() {
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(weapon->renderer);
    weapon->collider->setCenterOffset(Vec2(0, weapon->refTransform->size.y / 2 + weapon->transform->size.y / 2));
    if(weapon->type == WeaponType::SWORD) weapon->transform->angle = -45;
    if(weapon->id == 1) {
        weapon->transform->angle = -45;
        Vec2 textureSize = renderer->loadTexture("sword", "resources/textures/sword.png");
        renderer->addToState("attack", "sword", TextureDataBuilder::init(TextureType::TEXTURE, "sword", textureSize).build());
        renderer->setState("attack");
    }
    if(weapon->id == 2) {
        Vec2 textureSize = renderer->loadTexture("sword", "resources/textures/sword.png");
        renderer->addToState("attack", "sword", TextureDataBuilder::init(TextureType::TEXTURE, "sword", textureSize).build());
        renderer->setState("attack");
    }
    Object::addProjectile(weapon);
    return weapon;
}