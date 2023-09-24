#include "player.h"

Player& Player::spawn(Vec2 pos, Vec2 size) {
    Entity::spawn(pos, size);
    renderer.loadAnimation("move_left", "resources/textures/NPC_188.png", 6, Vec2(34, 48));
    renderer.loadTexture("idle", "resources/textures/Gore_16.png");
    renderer.loadTextureAtlas("idk_atlas", "resources/textures/Item_521.png", Vec2(22, 28));
    renderer.loadAnimation("idk_anum", "resources/textures/Item_521.png", 6, Vec2(22, 28));
    renderer.loadAnimation("idk_anum_flipped", "resources/textures/Item_521.png", 6, Vec2(22, 28), true);
    renderer.loadAnimation("move_right", "resources/textures/NPC_188.png", 6, Vec2(34, 48), true);
    renderer.setMain("idle", RendererType::TEXTURE);
    return *this;
}

void Player::update() {
    physics.accel = Vec2(0, 0);
    renderer.setMain("idle", RendererType::TEXTURE);
    if (IsKeyDown(KEY_SPACE)) {
        physics.accel += Vec2(0, -1.5);
        //renderer.setMain("idk_atlas", RendererType::TEXTURE_ATLAS, Vec2(0, 3));
    }
    if (IsKeyDown(KEY_A)) {
        physics.accel += Vec2(-1, 0);
        renderer.setMain("move_left", RendererType::ANIMATION);
    }
    if (IsKeyDown(KEY_D)) {
        physics.accel += Vec2(1, 0);
        renderer.setMain("move_right", RendererType::ANIMATION);
    }

    if (IsKeyPressed(KEY_SPACE)) renderer.addExtra("idk_anum", RendererType::ANIMATION);
    if (IsKeyReleased(KEY_SPACE)) renderer.removeExtra("idk_anum");

    physics.applyAccel();
}

void Player::onCollision(Tile* other) {
    physics.speed.y = 0;
}

void Player::onCollision(Entity* other) {
    
}