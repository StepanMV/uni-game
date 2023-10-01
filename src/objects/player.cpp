#include "player.h"
#include "tile.h"
#include "level.h"

Player::Player(Vec2 pos, Vec2 size): Entity(pos, size) { }

Player& Player::spawn(Vec2 pos, Vec2 size) {
    Entity::spawn(pos, size);
    renderer.loadTexture("legs", "resources/textures/Armor_Legs_3.png");
    renderer.loadTexture("head", "resources/textures/Armor_Head_3.png");
    renderer.loadTexture("other", "resources/textures/Armor_3.png");

    renderer.addToState("idle", "head").spriteSheet({1, 20}, {0, 0}); // голова
    renderer.addToState("idle", "legs").spriteSheet({1, 20}, {0, 0}); // ноги
    renderer.addToState("idle", "other").spriteSheet({9, 4}, {8, 0}); // задняя рука
    renderer.addToState("idle", "other").spriteSheet({9, 4}, {0, 0}); // тело
    renderer.addToState("idle", "other").spriteSheet({9, 4}, {7, 0}); // передняя рука
    renderer.addToState("idle", "other").spriteSheet({9, 4}, {0, 1}); // плечо

    renderer.addToState("move", "head").spriteSheet({1, 20}, {0, 0});
    renderer.addToState("move", "legs").animation({1, 20}, {0, 6}, {0, 19}, 28);
    renderer.addToState("move", "other").animation({9, 4}, {3, 3}, {6, 3}, 8);
    renderer.addToState("move", "other").spriteSheet({9, 4}, {0, 0});
    renderer.addToState("move", "other").animation({9, 4}, {3, 1}, {6, 1}, 8);
    renderer.addToState("move", "other").spriteSheet({9, 4}, {0, 1});

    renderer.addToState("jump", "legs").spriteSheet({1, 20}, {0, 5}); // ноги
    renderer.addToState("jump", "other").spriteSheet({9, 4}, {2, 3}); // задняя рука
    renderer.addToState("jump", "other").spriteSheet({9, 4}, {1, 0}); // тело
    renderer.addToState("jump", "head").spriteSheet({1, 20}, {0, 0}); // голова
    renderer.addToState("jump", "other").spriteSheet({9, 4}, {2, 1}); // передняя рука

    return *this;
}

void Player::update() {
    renderer.setState("idle");
    physics.accel = Vec2(0, 0);
    if (IsKeyPressed(KEY_SPACE)) {
        if(physics.onGround) {
            startY = pos.y;
            physics.speed.y = -20;
            physics.onGround = false;
            physics.jump = true;
        }
    }
    if(IsKeyDown(KEY_SPACE)) {
        renderer.setState("jump");
        if((startY - pos.y <= 400) && (physics.jump)) {
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
        physics.accel += Vec2(-1, 0);
        
    }
    if (IsKeyDown(KEY_D)) {
        physics.accel += Vec2(1, 0);
    }
    if(IsKeyDown(KEY_W)) {
        physics.accel += Vec2(0, -2.5);
    }
    physics.applyAccel();
    physics.onGround = false;
    if (physics.speed.x > 0) {
        renderer.setState("move", false, physics.speed.x / 10);  
    } else if (physics.speed.x < 0) {
        renderer.setState("move", true, -physics.speed.x / 10);  
    }
}

void Player::onCollision(Tile& other) {
    if(other.getId() == 0) {
        return;
    }
    if((!other.isUp) && (physics.speed.y > 0) && (pos.y + size.y / 2 < other.getPos().y)) {
        physics.speed.y = 0;
        physics.onGround = true;
        pos.y = other.getPos().y - other.getSize().y / 2 - size.y / 2 + 1;
    }
    else if((!other.isDown) && (physics.speed.y < 0) && (pos.y - size.y / 2 > other.getPos().y)){
        physics.speed.y = 0;
        physics.jump = false;
        pos.y = other.getPos().y + other.getSize().y / 2 + size.y / 2 - 1;
    }
    else if((!other.isLeft) && (physics.speed.x > 0) && (pos.x + size.x / 2 < other.getPos().x)) {
        if(pos.y <= other.getPos().y - other.getSize().y / 2) {
             pos.y = other.getPos().y - other.getSize().y / 2 - size.y / 2;
        }
        else {
            physics.speed.x = 0;
            pos.x = other.getPos().x - other.getSize().x / 2 - size.x / 2 + 1;
        }
    }
    else if((!other.isRight) && (physics.speed.x < 0) && (pos.x - size.x / 2 > other.getPos().x)) {
        if(pos.y <= other.getPos().y - other.getSize().y / 2) {
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