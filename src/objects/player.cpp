#include "player.h"
#include "tile.h"

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
    if (IsKeyDown(KEY_SPACE)) {
        if(physics.onGround) {
            startY = pos.y;
            physics.speed.y = -20;
            physics.onGround = false;
            physics.fly = true;
        }
        if((startY - pos.y <= 400) && (physics.fly)) {
            physics.accel += Vec2(0, -2.5);
        }
        else {
            physics.fly = false;
        }
    }
    else if(IsKeyReleased(KEY_SPACE)) {
        physics.fly = false;
    }
    if (IsKeyDown(KEY_A)) {
        renderer.setMain("move_left", RendererType::ANIMATION);
        physics.accel += Vec2(-1, 0);
    }
    if (IsKeyDown(KEY_D)) {
        renderer.setMain("move_right", RendererType::ANIMATION);
        physics.accel += Vec2(1, 0);
    }
    if(IsKeyDown(KEY_W)) {
        physics.accel += Vec2(0, -2.5);
    }
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Tile tile(Vec2(GetMouseX(), GetMouseY()), Vec2(64, 64));
        level->placeTile(tile);
    }
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        Tile tile(Vec2(GetMouseX(), GetMouseY()), Vec2(64, 64));
        level->breakTile(tile);
    }
    physics.applyAccel();
    physics.onGround = false;
}

void Player::setLevel(Level* level) {
    this->level = level;
}

void Player::onCollision(Tile* other) {
    if((physics.speed.y > 0) && (pos.y + size.y / 2 < other->getPos().y)) {
        physics.speed.y = 0;
        physics.onGround = true;
        pos.y = other->getPos().y - other->getSize().y / 2 - size.y / 2 + 1;
    }
    else if((physics.speed.y < 0) && (pos.y - size.y / 2 > other->getPos().y)){
        physics.speed.y = 0;
        pos.y = other->getPos().y + other->getSize().y / 2 + size.y / 2;
    }
    else if((physics.speed.x > 0) && (pos.x + size.x / 2 < other->getPos().x)) {
        if(pos.y + size.y / 6 < other->getPos().y - other->getSize().y / 2) {
            pos.y = other->getPos().y - other->getSize().y / 2 - size.y / 2 + 1;
        }
        else {
            physics.speed.x = 0;
            pos.x = other->getPos().x - other->getSize().x / 2 - size.x / 2;
        }
    }
    else if((physics.speed.x < 0) && (pos.x - size.x / 2 > other->getPos().x)) {
        if(pos.y + size.y / 6 < other->getPos().y - other->getSize().y / 2) {
            pos.y = other->getPos().y - other->getSize().y / 2 - size.y / 2 + 1;
        }
        else {
            physics.speed.x = 0;
            pos.x = other->getPos().x + other->getSize().x / 2 + size.x / 2;
        }
    }
}

void Player::onCollision(Entity* other) {
    
}