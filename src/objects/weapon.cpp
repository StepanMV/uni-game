#include "weapon.h"
#include "game.h"

void Weapon::update() {
    if(id == 1) {
        transform->angle += attackSpeed;
    }
    if(id == 2) {
        Vector2 mousePos = GetMousePosition();
        mousePos = GetScreenToWorld2D(mousePos, Game::camera->getCamera());
        transform->angle = atan2(mousePos.y - refTransform->pos.y, mousePos.x - refTransform->pos.x) * 180 / M_PI + 90;
        leftSide = transform->angle < 0 || transform->angle > 180;
    }
    if(timer->isDone()) {
        id = 0;
    }
    this->collider->setPos(Vec2(refTransform->pos.x, refTransform->pos.y - refTransform->size.y / 2 - transform->size.y / 2));
    collider->calcHitbox();
}

void Weapon::render() {
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(this->renderer);
    renderer->setRotation(transform->angle);
    renderer->setFlipped(leftSide);
    renderer->render();
}

void Weapon::setLeftSide(bool _leftSide) {
    if(_leftSide != leftSide) {
        transform->angle = -transform->angle;
        attackSpeed = -attackSpeed;
    }
    leftSide = _leftSide;
}

WeaponBuilder WeaponBuilder::spawn(std::shared_ptr<MyTransform> refTransform, unsigned id, Vec2 size) {
    WeaponBuilder builder;
    builder.weapon = std::shared_ptr<Weapon>(new Weapon());
    builder.weapon->refTransform = refTransform;
    builder.weapon->id = id;
    builder.weapon->transform->size = size;
    builder.weapon->renderer = std::make_shared<CoolRenderer>(builder.weapon->transform);
    builder.weapon->tileCollide = false;
    builder.weapon->physics = std::make_shared<Physics>();
    return builder;
}

WeaponBuilder& WeaponBuilder::extra(double _attackTime, unsigned _damage, bool _fromPlayer) {
    weapon->damage = _damage;
    weapon->attackSpeed = 180 / (_attackTime * 60);
    weapon->timer = Timer::getInstance(_attackTime);
    weapon->fromPlayer = _fromPlayer;
    return *this;
}

std::shared_ptr<Weapon> WeaponBuilder::build() {
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(weapon->renderer);
    weapon->collider->setCenterOffset(Vec2(0, weapon->refTransform->size.y / 2 + weapon->transform->size.y / 2));
    if(weapon->id == 1) {
        weapon->transform->angle = -45;
        renderer->loadTexture("sword", "resources/textures/sword.png");
        Vec2 textureSize = renderer->getTextureSize("sword");
        renderer->addToState("attack", "sword", TextureDataBuilder::init(TextureType::TEXTURE, "sword", textureSize).build());
        renderer->setState("attack");
    }
    if(weapon->id == 2) {
        renderer->loadTexture("sword", "resources/textures/sword.png");
        Vec2 textureSize = renderer->getTextureSize("sword");
        renderer->addToState("attack", "sword", TextureDataBuilder::init(TextureType::TEXTURE, "sword", textureSize).build());
        renderer->setState("attack");
    }
    Object::objects.push_back(weapon);
    return weapon;
}