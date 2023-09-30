#include "player.h"
#include "tile.h"
#include "level.h"

Player::Player(Vec2 pos, Vec2 size): Entity(pos, size) { }

Player& Player::spawn(Vec2 pos, Vec2 size) {
    Entity::spawn(pos, size);
    renderer.loadAnimation("move_left", "resources/textures/NPC_188.png", 6, Vec2(34, 48));
    renderer.loadTexture("idle", "resources/textures/Gore_16.png");
    renderer.loadTextureAtlas("idk_atlas", "resources/textures/Item_521.png", Vec2(22, 28));
    renderer.loadAnimation("idk_anum", "resources/textures/Item_521.png", 6, Vec2(22, 28));
    renderer.loadAnimation("idk_anum_flipped", "resources/textures/Item_521.png", 6, Vec2(22, 28), true);
    renderer.loadAnimation("move_right", "resources/textures/NPC_188.png", 6, Vec2(34, 48), true);
    renderer.setMain("idle", RendererType::TEXTURE);
    renderer.setMain("idk_atlas", RendererType::TEXTURE_ATLAS, Vec2(0, 0));
    return *this;
}

void Player::update() {
    physics.accel = Vec2(0, 0);
    renderer.setMain("idle", RendererType::TEXTURE);
    if (IsKeyPressed(KEY_SPACE)) {
        if(physics.onGround) {
            startY = pos.y;
            physics.speed.y = -20;
            physics.onGround = false;
            physics.jump = true;
        }
    }
    if(IsKeyDown(KEY_SPACE)) {
        if((startY - pos.y <= 100) && (physics.jump)) {
            physics.accel += Vec2(0, -2.5);
        }
        else {
            physics.jump = false;
        }
    }
    else if(IsKeyReleased(KEY_SPACE)) {
        physics.jump = false;
    }
    if (IsKeyDown(KEY_A)) {
        renderer.setMain("move_left", RendererType::ANIMATION);
        physics.accel += Vec2(-0.3, 0);
    }
    if (IsKeyDown(KEY_D)) {
        renderer.setMain("move_right", RendererType::ANIMATION);
        physics.accel += Vec2(0.3, 0);
    }
    if(IsKeyDown(KEY_W)) {
        physics.accel += Vec2(0, -2.5);
    }
    physics.applyAccel();
    physics.onGround = false;
}

void Player::onBoard() {
    if(pos.x - size.x / 2 < Level::levelOffset * Level::tileSize) {
        physics.speed.x = 0;
        pos.x = Level::levelOffset * Level::tileSize + size.x / 2;
    }
    if(pos.y - size.y / 2 < Level::levelOffset * Level::tileSize) {
        physics.speed.y = 0;
        pos.y = Level::levelOffset * Level::tileSize + size.y / 2;
        physics.jump = false;
    }
    if(pos.x + size.x / 2 > (Level::levelSizeX - Level::levelOffset) * Level::tileSize) {
        physics.speed.x = 0;
        pos.x = (Level::levelSizeX - Level::levelOffset) * Level::tileSize - size.x / 2;
    }
    if(pos.y + size.y / 2 > (Level::levelSizeY - Level::levelOffset) * Level::tileSize)
    {
        physics.speed.y = 0;
        pos.y = (Level::levelSizeY - Level::levelOffset) * Level::tileSize - size.y / 2;
        physics.onGround = true;
    }
}

void Player::onCollision(Tile& other) {
    if(other.getId() == 0) {
        return;
    }
    if((!other.isUp) && (physics.speed.y > 0) && (pos.y + size.y / 2 < other.getPos().y + other.getSize().y / 2)) {
        physics.speed.y = 0;
        physics.onGround = true;
        pos.y = other.getPos().y - other.getSize().y / 2 - size.y / 2 + 1;
    }
    if((!other.isDown) && (physics.speed.y < 0) && (pos.y - size.y / 2 > other.getPos().y - other.getSize().y / 2)){
        physics.speed.y = 0;
        physics.jump = false;
        pos.y = other.getPos().y + other.getSize().y / 2 + size.y / 2 - 1;
    }
    if((!other.isLeft) && (physics.speed.x > 0) && (pos.x + size.x / 2 < other.getPos().x + other.getSize().x / 2)) {
        if((!other.isUp) && (pos.y <= other.getPos().y - other.getSize().y / 2)) {
            pos.y = other.getPos().y - other.getSize().y / 2 - size.y / 2;
        }
        else {
            physics.speed.x = 0;
            pos.x = other.getPos().x - other.getSize().x / 2 - size.x / 2 + 1;
        }
    }
    if((!other.isRight) && (physics.speed.x < 0) && (pos.x - size.x / 2 > other.getPos().x - other.getSize().x / 2)) {
        if((!other.isUp) && (pos.y <= other.getPos().y - other.getSize().y / 2)) {
            pos.y = other.getPos().y - other.getSize().y / 2 - size.y / 2;
        }
        else {
            physics.speed.x = 0;
            pos.x = other.getPos().x + other.getSize().x / 2 + size.x / 2 - 1;
        }
    }
}

void Player::onCollision(Entity& other) {
    
}