#include "player.h"

Player& Player::spawn(Vec2 pos, Vec2 size) {
    Entity::spawn(pos, size);
    renderer.saveSprite("idle", "resources/sprites/player.png");
    return *this;
}

void Player::update() {
    physics.accel = Vec2(0, 0);
    if (IsKeyDown(KEY_SPACE)) {
        physics.accel += Vec2(0, -1.5);
    }
    if (IsKeyDown(KEY_A)) {
        renderer.setState("idle", true);
        physics.accel += Vec2(-1, 0);
    }
    if (IsKeyDown(KEY_D)) {
        renderer.setState("idle");
        physics.accel += Vec2(1, 0);
    }

    physics.applyAccel();
}

void Player::onCollision(Tile* other) {
    physics.speed.y = 0;
}

void Player::onCollision(Entity* other) {
    
}