#include "bosses.h"
#include "projectile.h"
#include "player.h"
#include <iostream>
#include "audio.h"
#include "particle.h"

std::shared_ptr<Slime> Slime::spawn(Vec2 pos, std::shared_ptr<Player> target) {
    std::shared_ptr<Slime> slime = std::shared_ptr<Slime>(new Slime());

    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(slime->renderer);
    Vec2 textureSize = renderer->loadTexture("Slime", "resources/textures/Slime.png");
    renderer->addToState("idle", "Slime", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "Slime", textureSize)
    .spriteSheet({1, 2}, {0, 0}).build());
    renderer->addToState("jump", "Slime", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "Slime", textureSize)
    .spriteSheet({1, 2}, {0, 1}).build());
    renderer->setState("idle");

    slime->readStats("Slime");

    slime->transform->pos = pos;
    slime->target = target;

    Object::addEnemy(slime);
    return slime;
}

std::shared_ptr<KingSlime> KingSlime::spawn(Vec2 pos, std::shared_ptr<Player> target) {
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

    Object::addEnemy(kingSlime);
    return kingSlime;
}

std::shared_ptr<Eye> Eye::spawn(Vec2 pos, std::shared_ptr<Player> target) {
    std::shared_ptr<Eye> eye = std::shared_ptr<Eye>(new Eye());

    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(eye->renderer);
    Vec2 textureSize = renderer->loadTexture("Eye", "resources/textures/Eye.png");
    renderer->addToState("idle", "Eye", TextureDataBuilder::init(TextureType::ANIMATION, "Eye", textureSize)
    .animation({1, 2}, {0, 0}, {0, 1}, 4).build());
    renderer->setState("idle");

    eye->readStats("Eye");

    eye->transform->pos = pos;
    eye->target = target;

    Object::addEnemy(eye);
    return eye;
}

std::shared_ptr<EyeOfCtulhu> EyeOfCtulhu::spawn(Vec2 pos, std::shared_ptr<Player> target) {
    std::shared_ptr<EyeOfCtulhu> eyeOfCtulhu = std::shared_ptr<EyeOfCtulhu>(new EyeOfCtulhu());

    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(eyeOfCtulhu->renderer);
    Vec2 textureSize = renderer->loadTexture("EyeOfCtulhu", "resources/textures/EyeOfCtulhu.png");
    renderer->addToState("phase1", "EyeOfCtulhu", TextureDataBuilder::init(TextureType::ANIMATION, "EyeOfCtulhu", textureSize)
    .animation({1, 6}, {0, 0}, {0, 2}, 4).build());
    renderer->addToState("phase2", "EyeOfCtulhu", TextureDataBuilder::init(TextureType::ANIMATION, "EyeOfCtulhu", textureSize)
    .animation({1, 6}, {0, 3}, {0, 5}, 4).build());
    renderer->setState("phase1");

    eyeOfCtulhu->readStats("EyeOfCtulhu");

    eyeOfCtulhu->transform->pos = pos;
    eyeOfCtulhu->target = target;
    eyeOfCtulhu->maxFriction = eyeOfCtulhu->physics->friction;

    Object::addEnemy(eyeOfCtulhu);
    return eyeOfCtulhu;
}

std::shared_ptr<EowSegment> EowSegment::spawn(Vec2 pos, std::shared_ptr<Player> target) {
    std::shared_ptr<EowSegment> eowSegment = std::shared_ptr<EowSegment>(new EowSegment());

    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(eowSegment->renderer);
    Vec2 textureSize = renderer->loadTexture("EowTail", "resources/textures/EowTail.png");
    renderer->addToState("tail", "EowTail", TextureDataBuilder::init(TextureType::TEXTURE, "EowTail", textureSize).build());
    textureSize = renderer->loadTexture("EowSegment", "resources/textures/EowSegment.png");
    renderer->addToState("segment", "EowSegment", TextureDataBuilder::init(TextureType::TEXTURE, "EowSegment", textureSize)
    .build());
    renderer->setState("segment");

    eowSegment->readStats("EowSegment");

    eowSegment->transform->pos = pos;
    eowSegment->target = target;

    Object::addEnemy(eowSegment);
    return eowSegment;
}

std::shared_ptr<EowHead> EowHead::spawnHead(Vec2 pos, std::shared_ptr<Player> target) {
    std::shared_ptr<EowHead> eowHead = std::shared_ptr<EowHead>(new EowHead());
    eowHead->nextSegment = nullptr;

    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(eowHead->renderer);
    Vec2 textureSize = renderer->loadTexture("EowHead", "resources/textures/EowHead.png");
    renderer->addToState("idle", "EowHead", TextureDataBuilder::init(TextureType::TEXTURE, "EowHead", textureSize)
    .build());
    renderer->setState("idle");
    eowHead->readStats("EowHead");

    eowHead->transform->pos = pos;
    eowHead->target = target;
    Object::addEnemy(eowHead);
    return eowHead;
}

std::shared_ptr<EowHead> EowHead::spawn(Vec2 pos, std::shared_ptr<Player> target) {
    std::shared_ptr<EowHead> eowHead = spawnHead(pos, target);
    std::shared_ptr<EowSegment> segment = eowHead;
    for(int i = 0; i < 20; i++) {
        std::shared_ptr<EowSegment> eowSegment;
        eowSegment = EowSegment::spawn(pos, target);
        eowSegment->nextSegment = segment;
        segment->prevSegment = eowSegment;
        segment = eowSegment;
    }
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
        Vec2 direction = (target->getPos() - transform->pos);
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
    if(betweenTpTimer->isDone() && onGround) {
        if(!tpTimer->isDone()) {
            collider->setScale(currentScale * tpTimer->getTimeLeft() / tpTime);
            physics->accel.x = 0;
            ParticleBuilder::init("Particle_2", transform->pos, 1.5, Color{0, 146, 255, 255})
                .setAmount(30, 0.5f * transform->size)
                .setFadeTime(0.5, 100, false)
                .setPhys(Vec2(200, 200), 0, 0)
                .build();
            return;
        }
        else {
            betweenTpTimer->reset();
            collider->setPos(tpPos);
            collider->setScale(currentScale);
            onGround = false;                           
        }
    }
    else {
        tpPos = target->getPos() - Vec2(0, transform->size.y / 2);
        tpTimer->reset();
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
            Audio::playSound("RawrXD");
        }
        return;
    }
    if(!chaseTimer->isDone()) {
        if(eyeSpawnTimer->isDone()) {
            Eye::spawn(transform->pos, target);
            eyeSpawnTimer->reset();
        }
        Vec2 direction = (target->getPos() - transform->pos);
        if(abs(direction.x) < 5) {
            physics->speed *= 0;
        }
        else {
            physics->speed *= 0.2;
            transform->angle = atan2(direction.y, direction.x) * 180 / 3.14 - 90;
        }
        direction += Vec2(0, -target->getSize().y * 5);
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
            Vec2 direction = target->getPos() - transform->pos;
            if(dashCount != 0) {
                dash(direction, 30);
                Audio::playSound("RawrXD");
            }
            transform->angle = atan2(direction.y, direction.x) * 180 / 3.14 - 90;
            betweenDashesTimer->reset();
            dashCount--;
        }
    }
}

void EyeOfCtulhu::phase2() {
    if(!chaseTimer->isDone()) {
        Vec2 direction = (target->getPos() - transform->pos);
        physics->speed *= 0.2;
        transform->angle = atan2(direction.y, direction.x) * 180 / 3.14 - 90;
        direction += Vec2(0, -target->getSize().y * 5);
        if(transform->pos.x < target->getPos().x) {
            direction -= Vec2(target->getSize().x * 3, 0);
        }
        else {
            direction += Vec2(target->getSize().x * 3, 0);
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
            physics->friction = maxFriction;
        }
        if(dashTimer->isDone()) {
            Vec2 direction = target->getPos() - transform->pos;
            transform->angle = atan2(direction.y, direction.x) * 180 / 3.14 - 90;
            if(dashCount != 0) {
                dash(direction, 20);
                Audio::playSound("RawrXD", 1, 1.5);
            }
            dashTimer->reset();
            dashCount--;
        }
    }
}

void EyeOfCtulhu::switchPhase() {
    physics->accel = Vec2(0, 0);
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(this->renderer);
    if(switchTimer->getTimeLeft() > switchTime / 2) {
        if(rotateSpeed < 20) rotateSpeed += 0.2;
    }
    else {
        if(rotateSpeed > 0.5) rotateSpeed -= 0.2;
        renderer->setState("phase2");
    }
    transform->angle += rotateSpeed;
}

void Eye::update() {
    Vec2 direction = (target->getPos() - transform->pos);
    physics->accel.normalize();
    transform->angle = atan2(direction.y, direction.x) * 180 / 3.14 - 90;
    direction.normalize();
    direction *= 0.5;
    move(direction);
    collider->calcHitbox();
}

void EowSegment::update() {
    if(!nextSegment || !nextSegment->isAlive()) {
        destroy();
        std::shared_ptr<EowHead> newHead = EowHead::spawnHead(transform->pos, target);
        newHead->transform->angle = transform->angle;
        newHead->health = health;
        if(prevSegment) prevSegment->nextSegment = newHead;
        newHead->prevSegment = prevSegment;
        return;
    }
    if(!prevSegment || !prevSegment->isAlive()) {
        std::shared_ptr<CoolRenderer> renderer = std::dynamic_pointer_cast<CoolRenderer>(this->renderer);
        renderer->setState("tail");
    }
    Vec2 distance = nextSegment->transform->pos - transform->pos;
    distance.normalize();
    transform->angle = atan2(distance.y, distance.x) * 180 / 3.14 + 90;
    distance *= 4 * transform->size.y / 10;
    collider->setPos(nextSegment->transform->pos - distance);
    if(projTimer->isDone()) {
        auto proj = Projectile::spawn(9, transform->pos, false);
        proj->setDirection(target->getPos() - transform->pos);
        projTimer = Timer::getInstance((float) GetRandomValue(5, 20) / 10);
    }
    collider->calcHitbox();
}

void EowHead::update() {
    if(!prevSegment || !prevSegment->isAlive()) {
        destroy();
        return;
    }
    Vec2 direction = (target->getPos() - transform->pos);
    if(chaseTimer->isDone()) {
        if(!restTimer->isDone()) {
            Vec2 restOffset = Vec2(0, target->getSize().y * 10);
            restOffset.rotate(GetRandomValue(0, 360));
            direction += restOffset;
        }
        else {
            chaseTimer->reset();
        }
    }
    else {
        if (moveSoundTimer->isDone()) {
            Audio::playSound("WormMove");
            float timerVal = (target->getPos() - transform->pos).length() / 1000;
            if (timerVal > 1) timerVal = 1;
            if (timerVal < 0.15) timerVal = 0.15;
            moveSoundTimer = Timer::getInstance(timerVal);
        }
        restTimer->reset();
    }
    direction.normalize();
    Vec2 accel = physics->accel;
    move(direction);
    double accelAngle = angle(physics->accel, accel);
    if(accel.x * physics->accel.y - accel.y * physics->accel.x < 0) accelAngle *= -1;
    if(accelAngle > rotateAngle) {
        physics->accel.rotate(rotateAngle - accelAngle);    
    }
    if(accelAngle < -rotateAngle) {
        physics->accel.rotate(-rotateAngle - accelAngle);
    }
    physics->accel.normalize();
    transform->angle = atan2(physics->speed.y, physics->speed.x) * 180 / 3.14 + 90;
    collider->calcHitbox();
}