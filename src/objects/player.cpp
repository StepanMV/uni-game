#include "player.h"

Player& Player::spawn(Vec2 pos, Vec2 size) {
    Entity::spawn(pos, size);
    renderer.loadAnimation("idle", "resources/textures/NPC_188.png", 6, Vec2(34, 48));
    renderer.loadAnimation("idle_flip", "resources/textures/NPC_188.png", 6, Vec2(34, 48), true);
    renderer.setMain("idle", RendererType::ANIMATION);
    return *this;
}

void Player::update() {
    physics.accel = Vec2(0, 0);
    if (IsKeyDown(KEY_SPACE)) {
        physics.accel += Vec2(0, -1.5);
    }
    if (IsKeyDown(KEY_A)) {
        physics.accel += Vec2(-1, 0);
        renderer.setMain("idle", RendererType::ANIMATION);
    }
    if (IsKeyDown(KEY_D)) {
        physics.accel += Vec2(1, 0);
        renderer.setMain("idle_flip", RendererType::ANIMATION);
    }

    physics.applyAccel();
}

void Player::onCollision(Tile* other) {
    physics.speed.y = 0;
}

void Player::onCollision(Entity* other) {
    
}