#include "bosses.h"
#include <iostream>

std::shared_ptr<Slime> Slime::spawn(Vec2 pos, std::shared_ptr<MyTransform> target) {
    std::shared_ptr<Slime> slime = std::shared_ptr<Slime>(new Slime());

    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(slime->renderer);
    Vec2 textureSize = renderer->loadTexture("Slime", "resources/textures/KingSlime.png");
    renderer->addToState("idle", "Slime", TextureDataBuilder::init(TextureType::ANIMATION, "Slime", textureSize)
    .animation({1, 6}, {0, 0}, {0, 4}, 5).build());
    renderer->addToState("jump", "Slime", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "Slime", textureSize)
    .spriteSheet({1, 6}, {0, 5}).build());
    renderer->setState("idle");

    slime->readStats("Slime");

    slime->transform->pos = pos;
    slime->target = target;

    Object::objects.push_back(slime);
    return slime;
}

std::shared_ptr<KingSlime> KingSlime::spawn(Vec2 pos, std::shared_ptr<MyTransform> target) {
    std::shared_ptr<KingSlime> kingSlime = std::shared_ptr<KingSlime>(new KingSlime());

    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(kingSlime->renderer);
    Vec2 textureSize = renderer->loadTexture("KingSlime", "resources/textures/KingSlime.png");
    renderer->addToState("idle", "KingSlime", TextureDataBuilder::init(TextureType::ANIMATION, "KingSlime", textureSize)
    .animation({1, 6}, {0, 0}, {0, 4}, 5).build());
    renderer->addToState("jump", "KingSlime", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "KingSlime", textureSize)
    .spriteSheet({1, 6}, {0, 5}).build());
    renderer->setState("idle");

    kingSlime->readStats("KingSlime");

    kingSlime->transform->pos = pos;
    kingSlime->target = target;

    Object::objects.push_back(kingSlime);
    return kingSlime;
}

std::shared_ptr<Eye> Eye::spawn(Vec2 pos, std::shared_ptr<MyTransform> target) {
    std::shared_ptr<Eye> eye = std::shared_ptr<Eye>(new Eye());

    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(eye->renderer);
    Vec2 textureSize = renderer->loadTexture("Eye", "resources/textures/KingSlime.png");
    renderer->addToState("idle", "Eye", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "Eye", textureSize)
    .spriteSheet({1, 6}, {0, 0}).build());
    renderer->setState("idle");

    eye->readStats("Eye");

    eye->transform->pos = pos;
    eye->target = target;

    Object::objects.push_back(eye);
    return eye;
}

std::shared_ptr<EyeOfCtulhu> EyeOfCtulhu::spawn(Vec2 pos, std::shared_ptr<MyTransform> target) {
    std::shared_ptr<EyeOfCtulhu> eyeOfCtulhu = std::shared_ptr<EyeOfCtulhu>(new EyeOfCtulhu());

    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(eyeOfCtulhu->renderer);
    Vec2 textureSize = renderer->loadTexture("EyeOfCtulhu", "resources/textures/KingSlime.png");
    renderer->addToState("idle", "EyeOfCtulhu", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "EyeOfCtulhu", textureSize)
    .spriteSheet({1, 6}, {0, 0}).build());
    renderer->setState("idle");

    eyeOfCtulhu->readStats("EyeOfCtulhu");

    eyeOfCtulhu->transform->pos = pos;
    eyeOfCtulhu->target = target;

    Object::objects.push_back(eyeOfCtulhu);
    return eyeOfCtulhu;
}

std::shared_ptr<EowSegment> EowSegment::spawn(Vec2 pos, std::shared_ptr<MyTransform> target, bool isTail) {
    std::shared_ptr<EowSegment> eowSegment = std::shared_ptr<EowSegment>(new EowSegment());

    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(eowSegment->renderer);
    Vec2 textureSize = renderer->loadTexture("EowSegment", "resources/textures/KingSlime.png");
    //if(isTail) ...
    renderer->addToState("idle", "EowSegment", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "EowSegment", textureSize)
    .spriteSheet({1, 6}, {0, 0}).build());
    renderer->setState("idle");

    eowSegment->readStats("EowSegment");
    //if(isTail) eowSegment->readStats("EowTail")

    eowSegment->transform->pos = pos;
    eowSegment->target = target;

    Object::objects.push_back(eowSegment);
    return eowSegment;
}

std::shared_ptr<EowHead> EowHead::spawn(Vec2 pos, std::shared_ptr<MyTransform> target) {
    std::shared_ptr<EowHead> eowHead = std::shared_ptr<EowHead>(new EowHead());
    eowHead->nextSegment = nullptr;

    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(eowHead->renderer);
    Vec2 textureSize = renderer->loadTexture("EowHead", "resources/textures/KingSlime.png");
    renderer->addToState("idle", "EowHead", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "EowHead", textureSize)
    .spriteSheet({1, 6}, {0, 0}).build());
    renderer->setState("idle");

    eowHead->readStats("EowHead");
    std::shared_ptr<EowSegment> segment;
    for(int i = 0; i < 20; i++) {
        std::shared_ptr<EowSegment> eowSegment;
        if(i < 19) eowSegment = EowSegment::spawn(pos, target);
        else eowSegment = EowSegment::spawn(pos, target, true);
        if(i == 0) eowSegment->setNextSegment(eowHead);
        else eowSegment->setNextSegment(segment);
        segment = eowSegment;
    }

    eowHead->transform->pos = pos;
    eowHead->target = target;

    Object::objects.push_back(eowHead);
    return eowHead;
}

void Slime::slimeBehavior() {
    physics->accel.y = 0;
    if(onGround) {
        if(!stayTimer->isDone()) {
            physics->accel.x = 0;
            return;
        }
        else {
            jumpCount++;
            jumpCount %= 3;
            if(jumpCount == 0) {
                jumpTime *= 2;
            }
            else if(jumpCount == 1) {
                jumpTime /= 2;
            }
            jumpTimer = Timer::getInstance(jumpTime);
            stayTime = (double) GetRandomValue(5 , 30) / 10;
            stayTimer = Timer::getInstance(jumpTime + stayTime);
        }
        Vec2 direction = (target->pos - transform->pos);
        direction.y = 0;
        direction.normalize();
        direction *= 1.5;
        move(direction);
    }
    jump();
    onGround = false;
}

void KingSlime::update() {
    onBoard();
    if(betweenTpTimer->isDone() && onGround) {                  // if can tp and on ground
        if(!tpTimer->isDone()) {                                    // if tp is not done
            collider->setScale(currentScale * tpTimer->getTimeLeft() / tpTime);     // scale down relative to time left
            physics->accel.x = 0;                                   // stop moving
            return;                                                 // don't do anything else
        }
        else {                                          // if tp is done
            betweenTpTimer->reset();                        // reset timer between tps
            collider->setPos(tpPos);                        // set position to tp position
            collider->setScale(currentScale);                          // return scale to normal
            onGround = false;                           
        }
    }
    else {                                              // if can't tp
        tpPos = target->pos;                                // update tp position
        tpTimer->reset();                                   // reset tp timer
    }
    if(health >= max_health / 1.41) {
        currentScale = (double) health / max_health;
    }
    if(slimeSpanwTimer->isDone()) {
        int slimeCount = GetRandomValue(1, 6);
        for(int i = 0; i < slimeCount; i++) {
            Slime::spawn(transform->pos + Vec2(GetRandomValue(-transform->size.x / 3, transform->size.x / 3), GetRandomValue(-transform->size.y / 3, transform->size.y / 3)), target);
        }
        slimeSpanwTimer->reset();
    }
    collider->setScale(currentScale);
    slimeBehavior();
    collider->calcHitbox();
}

void Slime::render() {
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(this->renderer);
    renderer->setState("idle");
    if (!onGround) {
        renderer->setState("jump");
    }
}

void Slime::update() {
    onBoard();
    slimeBehavior();
    collider->calcHitbox();
}

void EyeOfCtulhu::update() {
    switch(phase) {
        case 1: {
            phase1();
            break;
        }
        case 2: {
            phase2();
            break;
        }
    }
    collider->calcHitbox();
}

void EyeOfCtulhu::phase1() {
    if(health <= max_health / 2) {
        if(!switchTimer) switchTimer = Timer::getInstance(switchTime);
        if(!switchTimer->isDone()) {
            switchPhase();
        }
        else {
            phase = 2;
            chaseTimer->reset();
            switchTimer = nullptr;
        }
        return;
    }
    if(!chaseTimer->isDone()) {
        if(eyeSpawnTimer->isDone()) {
            Eye::spawn(transform->pos, target);
            eyeSpawnTimer->reset();
        }
        Vec2 direction = (target->pos - transform->pos);
        if(abs(direction.x) < 5) {
            physics->speed *= 0;
        }
        else {
            physics->speed *= 0.2;
            transform->angle = atan(direction.y / direction.x) * 180 / M_PI;
        }
        direction += Vec2(0, -target->size.y * 5);
        direction.normalize();
        direction *= 0.9;
        move(direction);
        physics->accel *= 0.9;
    }
    else {
        physics->accel = Vec2(0, 0);
        if(dashCount == -1) {
            dashCount = 4;
            chaseTimer->reset();
        }
        if(physics->speed == Vec2(0, 0) && betweenDashesTimer->isDone()) {
            Vec2 direction = target->pos - transform->pos;
            if(dashCount != 0) dash(direction, 30);
            transform->angle = atan(direction.y / direction.x) * 180 / M_PI;
            betweenDashesTimer->reset();
            dashCount--;
        }
    }
}

void EyeOfCtulhu::phase2() {
    if(!chaseTimer->isDone()) {
        Vec2 direction = (target->pos - transform->pos);
        physics->speed *= 0.2;
        transform->angle = atan(direction.y / direction.x) * 180 / M_PI;
        direction += Vec2(0, -target->size.y * 5);
        if(transform->pos.x < target->pos.x) {
            direction -= Vec2(target->size.x * 3, 0);
        }
        else {
            direction += Vec2(target->size.x * 3, 0);
        }
        direction.normalize();
        direction *= 0.9;
        move(direction);
        physics->accel *= 0.9;
    }
    else {
        physics->accel = Vec2(0, 0);
        physics->friction = 0;
        if(dashCount == -1) {
            dashCount = 4;
            chaseTimer->reset();
            physics->friction = 0.5;
        }
        if(dashTimer->isDone()) {
            Vec2 direction = target->pos - transform->pos;
            transform->angle = atan(direction.y / direction.x) * 180 / M_PI;
            if(dashCount != 0) dash(direction, 20);
            dashTimer->reset();
            dashCount--;
        }
    }
}

void EyeOfCtulhu::switchPhase() {
    physics->accel = Vec2(0, 0);
    if(switchTimer->getTimeLeft() > switchTime / 2) {
        if(rotateSpeed < 20) rotateSpeed += 0.2;
    }
    else {
        if(rotateSpeed > 0.5) rotateSpeed -= 0.2;
        //setTexture(phase2);
    }
    transform->angle += rotateSpeed;
}

void EyeOfCtulhu::render() {
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(this->renderer);
    renderer->setState("idle");
}

void Eye::update() {
    Vec2 direction = (target->pos - transform->pos);
    physics->accel.normalize();
    transform->angle = atan(direction.y / direction.x) * 180 / M_PI;
    direction.normalize();
    direction *= 0.5;
    move(direction);
    collider->calcHitbox();
}

void Eye::render() {
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(this->renderer);
    renderer->setState("idle");
}

void EowSegment::update() {
    Vec2 distance = nextSegment->transform->pos - transform->pos;
    distance.normalize();
    transform->angle = atan(distance.y / distance.x) * 180 / M_PI;
    distance *= transform->size.x;
    collider->setPos(nextSegment->transform->pos - distance);
    collider->calcHitbox();
}

void EowSegment::render() {
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(this->renderer);
    renderer->setState("idle");
}

void EowSegment::setNextSegment(std::shared_ptr<EowSegment> nextSegment) {
    this->nextSegment = nextSegment;
}

void EowHead::update() {
    Vec2 direction = (target->pos - transform->pos);
    physics->accel.normalize();
    transform->angle = atan(direction.y / direction.x) * 180 / M_PI;
    direction.normalize();
    direction *= 0.5;
    move(direction);
    collider->calcHitbox();
}

void EowHead::render() {
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(this->renderer);
    renderer->setState("idle");
}