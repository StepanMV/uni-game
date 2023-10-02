#include "player.h"
#include "tile.h"
#include "level.h"
#include "keyboard.h"

void Player::update() {
    physics.accel = Vec2(0, 0);
    if (IsKeyPressed(KEY_SPACE)) {
        if(physics.onGround) {
            startY = pos.y;
            physics.speed.y = -20;
            physics.onGround = false;
            physics.jumping = true;
        }
    }
    if(IsKeyDown(KEY_SPACE)) {
        if((startY - pos.y <= 400) && (physics.jumping)) {
            physics.accel += Vec2(0, -2.5);
        }
        else {
            physics.jumping = false;
        }
    }
    else if(IsKeyReleased(KEY_SPACE)) {
        physics.jumping = false;
    }
    if (IsKeyDown(KEY_A)) {
        physics.accel += Vec2(-1, 0);
        
    }
    if (IsKeyDown(KEY_D)) {
        physics.accel += Vec2(1, 0);
    }
    if(Keyboard::isKeyDown(KEY_W)) {
        physics.accel += Vec2(0, -2.5);
    }
    physics.onGround = false;
}

void Player::render() {
    renderer.update(pos, size);
    renderer.setState("idle");

    if (physics.speed.x > 0) {
        renderer.setState("move", false, physics.speed.x / 10);  
    } else if (physics.speed.x < 0) {
        renderer.setState("move", true, -physics.speed.x / 10);
    }

    if (physics.jumping) {
        renderer.setState("jump");
    }
    renderer.render();
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
        physics.jumping = false;
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

PlayerBuilder PlayerBuilder::spawn(Vec2 pos, Vec2 size) {
    PlayerBuilder builder;
    builder.pos = pos;
    builder.size = size;
    return builder;
}

PlayerBuilder &PlayerBuilder::setMaxSpeeds(double maxMoveSpeed, double maxFallSpeed, double maxFlySpeed) {
    this->maxFallSpeed = maxFallSpeed;
    this->maxFlySpeed = maxFlySpeed;
    this->maxMoveSpeed = maxMoveSpeed;
    return *this;
}

PlayerBuilder &PlayerBuilder::setForces(double friction, double gravity) {
    this->friction = friction;
    this->gravity = gravity;
    return *this;
}

PlayerBuilder &PlayerBuilder::setHeadTexture(const std::string &texturePath) {
    headTexturePath = texturePath;
    return *this;
}

PlayerBuilder &PlayerBuilder::setLegsTexture(const std::string &texturePath) {
    legsTexturePath = texturePath;
    return *this;
}

PlayerBuilder &PlayerBuilder::setBodyTexture(const std::string &texturePath) {
    bodyTexturePath = texturePath;
    return *this;
}

Player PlayerBuilder::build() const
{
    Player player;

    player.pos = std::move(pos);
    player.size = std::move(size);

    player.physics.maxMoveSpeed = maxMoveSpeed;
    player.physics.maxFallSpeed = maxFallSpeed;
    player.physics.maxFlySpeed = maxFlySpeed;
    player.physics.friction = friction;
    player.physics.gravity = gravity;

    player.renderer.loadTexture("legs", legsTexturePath);
    player.renderer.loadTexture("head", headTexturePath);
    player.renderer.loadTexture("body", bodyTexturePath);

    player.renderer.addToState("idle", "head").spriteSheet({1, 20}, {0, 0}); // голова
    player.renderer.addToState("idle", "legs").spriteSheet({1, 20}, {0, 0}); // ноги
    player.renderer.addToState("idle", "body").spriteSheet({9, 4}, {8, 0}); // задняя рука
    player.renderer.addToState("idle", "body").spriteSheet({9, 4}, {0, 0}); // тело
    player.renderer.addToState("idle", "body").spriteSheet({9, 4}, {7, 0}); // передняя рука
    player.renderer.addToState("idle", "body").spriteSheet({9, 4}, {0, 1}); // плечо

    player.renderer.addToState("move", "head").spriteSheet({1, 20}, {0, 0});
    player.renderer.addToState("move", "legs").animation({1, 20}, {0, 6}, {0, 19}, 28);
    player.renderer.addToState("move", "body").animation({9, 4}, {3, 3}, {6, 3}, 8);
    player.renderer.addToState("move", "body").spriteSheet({9, 4}, {0, 0});
    player.renderer.addToState("move", "body").animation({9, 4}, {3, 1}, {6, 1}, 8);
    player.renderer.addToState("move", "body").spriteSheet({9, 4}, {0, 1});

    player.renderer.addToState("jump", "legs").spriteSheet({1, 20}, {0, 5}); // ноги
    player.renderer.addToState("jump", "body").spriteSheet({9, 4}, {2, 3}); // задняя рука
    player.renderer.addToState("jump", "body").spriteSheet({9, 4}, {1, 0}); // тело
    player.renderer.addToState("jump", "head").spriteSheet({1, 20}, {0, 0}); // голова
    player.renderer.addToState("jump", "body").spriteSheet({9, 4}, {2, 1}); // передняя рука

    return player;
}
