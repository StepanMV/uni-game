#include "player.h"
#include "tile.h"

Player::Player(Vec2 _pos, Vec2 _size): Entity(_pos, _size) {}

Player& Player::spawn() {
    Object::spawn();
    renderer.saveSprite("idle", "resources/sprites/player.png");
    return *this;
}

void Player::update() {
    physics.accel = Vec2(0, 0);
    if (IsKeyDown(KEY_SPACE)) {
        if(physics.onGround) {
            if(physics.speed.y > 0) {
                startY = pos.y;
                physics.speed.y = -20;
            }
            if(startY - pos.y <= 400) {
                physics.accel += Vec2(0, -2.5);
            }
            else {
                physics.onGround = false;
            }// иногда первый прыжок не срабатывает
        }
    }
    else if(IsKeyReleased(KEY_SPACE)) {
        physics.onGround = false;
    }
    if (IsKeyDown(KEY_A)) {
        renderer.setState("idle", true);
        physics.accel += Vec2(-1, 0);
    }
    if (IsKeyDown(KEY_D)) {
        renderer.setState("idle");
        physics.accel += Vec2(1, 0);
    }
    if(IsKeyDown(KEY_W)) {
        physics.accel += Vec2(0, -2.5);
    }
    physics.applyAccel();
}

void Player::onCollision(Tile* other) {
    if((physics.speed.y > 0) && (pos.y + size.y / 2 < other->getPos().y)) {
        physics.speed.y = 0;
        physics.onGround = true;
        pos.y = other->getPos().y - other->getSize().y / 2 - size.y / 2;
    }
    else if((physics.speed.y < 0) && (pos.y - size.y / 2 > other->getPos().y)){
        physics.speed.y = 0;
        pos.y = other->getPos().y + other->getSize().y / 2 + size.y / 2;
    }
    else if((physics.speed.x > 0) && (pos.x + size.x / 2 < other->getPos().x)) {
        physics.speed.x = 0;
        pos.x = other->getPos().x - other->getSize().x / 2 - size.x / 2;
    }
    else if((physics.speed.x < 0) && (pos.x - size.x / 2 > other->getPos().x)) {
        physics.speed.x = 0;
        pos.x = other->getPos().x + other->getSize().x / 2 + size.x / 2;
    }
}

void Player::onCollision(Entity* other) {
    
}