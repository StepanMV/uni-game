#include "player.h"
#include "tile.h"
#include "level.h"
#include "keyboard.h"
#include "projectile.h"

Player::Player() {
    renderer = std::make_shared<CoolRenderer>();
    physics = std::make_shared<Physics>();
}

void Player::update() {
    onBoard();
    physics->accel = Vec2(0, 0);
    if (IsKeyPressed(KEY_SPACE)) {
        if(physics->onGround) {
            startY = pos.y;
            physics->speed.y = -20;
            physics->onGround = false;
            physics->jumping = true;
        }
    }
    if(IsKeyDown(KEY_SPACE)) {
        if((startY - pos.y <= 100) && (physics->jumping)) {
            physics->accel += Vec2(0, -2.5);
        }
        else {
            physics->jumping = false;
        }
    }
    else if(IsKeyReleased(KEY_SPACE)) {
        physics->jumping = false;
    }
    if (IsKeyDown(KEY_A)) {
        physics->accel += Vec2(-1.5, 0);
        
    }
    if (IsKeyDown(KEY_D)) {
        physics->accel += Vec2(1.5, 0);
    }
    if(Keyboard::isKeyDown(KEY_W)) {
        physics->accel += Vec2(0, -2.5);
    }
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        isWeapon = true;
        if(isAttacking) {
            projTimer->reset();
        }
        if(projTimer->isDone()) {
            isAttacking = true;
        }
        else {
            isAttacking = false;
        }
    }
    else {
        isWeapon = false;
    }
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isAttacking = true;
    }
    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        isAttacking = false;
    }
    if (Keyboard::isKeyDown(KEY_S)) {
        physics->accel += Vec2(0, 2.5);
    }
    physics->onGround = false;
}

Projectile Player::getProjectile() const {
    if(isAttacking) {
        return Projectile(1, 1, true);
    }
    return Projectile(0, 1, true);
}

Projectile Player::getWeapon() const {
    if(isWeapon) return Projectile(2, 1, true);
    return Projectile(0, 1, true);
}

void Player::render() {
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(this->renderer);
    renderer->setState("idle");

    if (physics->speed.x > 0) {
        facingLeft = false;
        renderer->setState("move");
        renderer->setAnimationSpeed(10 * physics->speed.x / physics->maxMoveSpeed);
    } else if (physics->speed.x < 0) {
        facingLeft = true;
        renderer->setState("move");
        renderer->setAnimationSpeed(10 * -physics->speed.x / physics->maxMoveSpeed);
    }

    if (!physics->onGround) {
        renderer->setState("jump");
    }
    renderer->setFlipped(facingLeft);
    renderer->render();
}

void Player::onBoard() {
    if(pos.x - size.x / 2 < Level::levelOffset * Level::tileSize) {
        physics->speed.x = 0;
        pos.x = Level::levelOffset * Level::tileSize + size.x / 2;
    }
    if(pos.y - size.y / 2 < Level::levelOffset * Level::tileSize) {
        physics->speed.y = 0;
        pos.y = Level::levelOffset * Level::tileSize + size.y / 2;
        physics->jumping = false;
    }
    if(pos.x + size.x / 2 > (Level::levelSizeX - Level::levelOffset) * Level::tileSize) {
        physics->speed.x = 0;
        pos.x = (Level::levelSizeX - Level::levelOffset) * Level::tileSize - size.x / 2;
    }
    if(pos.y + size.y / 2 > (Level::levelSizeY - Level::levelOffset) * Level::tileSize)
    {
        physics->speed.y = 0;
        pos.y = (Level::levelSizeY - Level::levelOffset) * Level::tileSize - size.y / 2;
        physics->onGround = true;
    }
}

void Player::onCollision(Tile& other) {
    if(other.getId() == 0) {
        return;
    }
    if((!other.isUp) && (physics->speed.y > 0) && (pos.y + size.y / 2 < other.getPos().y + other.getSize().y / 2)) {
        if(!other.isRight && other.getPos().x + other.getSize().x / 2 - pos.x + size.x / 2 <= 1) {
            pos.x = other.getPos().x + other.getSize().x / 2 + size.x / 2 - 1;
        }
        else if(!other.isLeft && pos.x + size.x / 2 - other.getPos().x + other.getSize().x / 2 <= 1) {
            pos.x = other.getPos().x - other.getSize().x / 2 - size.x / 2 + 1;
        }
        else {
            physics->speed.y = 0;
            physics->onGround = true;
            pos.y = other.getPos().y - other.getSize().y / 2 - size.y / 2 + 1;
        }
    }
    if((!other.isDown) && (physics->speed.y < 0) && (pos.y - size.y / 2 > other.getPos().y - other.getSize().y / 2)){
        if(!other.isRight && other.getPos().x + other.getSize().x / 2 - pos.x + size.x / 2 <= 1) {
            pos.x = other.getPos().x + other.getSize().x / 2 + size.x / 2 - 1;
        }
        else if(!other.isLeft && pos.x + size.x / 2 - other.getPos().x + other.getSize().x / 2 <= 1) {
            pos.x = other.getPos().x - other.getSize().x / 2 - size.x / 2 + 1;
        }
        else {
            physics->speed.y = 0;
            physics->jumping = false;
            pos.y = other.getPos().y + other.getSize().y / 2 + size.y / 2 - 1;
        }
    }
    if((!other.isLeft) && (physics->speed.x > 0) && (pos.x + size.x / 2 < other.getPos().x + other.getSize().x / 2)) {
        if((other.canClimbLeft) && (pos.y <= other.getPos().y - other.getSize().y / 2)) {
            pos.y = other.getPos().y - other.getSize().y / 2 - size.y / 2 + 1;
        }
        else if(!other.isDown && other.getPos().y + other.getSize().y / 2 - pos.y + size.y / 2 <= 1) {
            pos.y = other.getPos().y + other.getSize().y / 2 + size.y / 2 - 1;
        }
        else {
            physics->speed.x = 0;
            pos.x = other.getPos().x - other.getSize().x / 2 - size.x / 2 + 1;
        }
    }
    if((!other.isRight) && (physics->speed.x < 0) && (pos.x - size.x / 2 > other.getPos().x - other.getSize().x / 2)) {
        if((other.canClimbRight) && (pos.y <= other.getPos().y - other.getSize().y / 2)) {
            pos.y = other.getPos().y - other.getSize().y / 2 - size.y / 2 + 1;
        }
        else if(!other.isDown && other.getPos().y + other.getSize().y / 2 - pos.y + size.y / 2 <= 1) {
            pos.y = other.getPos().y + other.getSize().y / 2 + size.y / 2 - 1;
        }
        else {
            physics->speed.x = 0;
            pos.x = other.getPos().x + other.getSize().x / 2 + size.x / 2 - 1;
        }
    }
}

void Player::onCollision(Entity& other) {
    
}

PlayerBuilder PlayerBuilder::spawn(Vec2 pos, Vec2 size) {
    PlayerBuilder builder;
    builder.player.pos = pos;
    builder.player.size = size;
    builder.player.renderer = std::make_shared<CoolRenderer>(&builder.player.pos);
    builder.player.physics = std::make_shared<Physics>();
    return builder;
}

PlayerBuilder &PlayerBuilder::setMaxSpeeds(double maxMoveSpeed, double maxFallSpeed, double maxFlySpeed) {
    player.physics->maxMoveSpeed = maxMoveSpeed;
    player.physics->maxFallSpeed = maxFallSpeed;
    player.physics->maxFlySpeed = maxFlySpeed;
    return *this;
}

PlayerBuilder &PlayerBuilder::setForces(double friction, double gravity) {
    player.physics->friction = friction;
    player.physics->gravity = gravity;
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

Player PlayerBuilder::build()
{
    if (headTexturePath.empty() || legsTexturePath.empty() || bodyTexturePath.empty()) return player;
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(player.renderer);

    renderer->loadTexture("legs", legsTexturePath);
    renderer->loadTexture("head", headTexturePath);
    renderer->loadTexture("body", bodyTexturePath);

    Vec2 headSize = renderer->getTextureSize("head");
    Vec2 legsSize = renderer->getTextureSize("legs");
    Vec2 bodySize = renderer->getTextureSize("body");


    renderer->addToState("idle", "head", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "head", headSize)
        .spriteSheet({1, 20}, {0, 0}).build());
    renderer->addToState("idle", "legs", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "legs", legsSize)
        .spriteSheet({1, 20}, {0, 0}).build());
    renderer->addToState("idle", "armBehind", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "body", bodySize)
        .spriteSheet({9, 4}, {8, 0}).build());
    renderer->addToState("idle", "body", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "body", bodySize)
        .spriteSheet({9, 4}, {0, 0}).build());
    renderer->addToState("idle", "armFront", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "body", bodySize)
        .spriteSheet({9, 4}, {7, 0}).build());
    renderer->addToState("idle", "shoulder", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "body", bodySize)
        .spriteSheet({9, 4}, {0, 1}).build());

    renderer->addToState("move", "head", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "head", headSize)
        .spriteSheet({1, 20}, {0, 0}).build());
    renderer->addToState("move", "legs", TextureDataBuilder::init(TextureType::ANIMATION, "legs", legsSize)
        .animation({1, 20}, {0, 6}, {0, 19}, 14).build());
    renderer->addToState("move", "armBehind", TextureDataBuilder::init(TextureType::ANIMATION, "body", bodySize)
        .animation({9, 4}, {3, 3}, {6, 3}, 4).build());
    renderer->addToState("move", "body", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "body", bodySize)
        .spriteSheet({9, 4}, {0, 0}).build());
    renderer->addToState("move", "armFront", TextureDataBuilder::init(TextureType::ANIMATION, "body", bodySize)
        .animation({9, 4}, {3, 1}, {6, 1}, 4).build());
    renderer->addToState("move", "shoulder", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "body", bodySize)
        .spriteSheet({9, 4}, {0, 1}).build());
    
    renderer->addToState("jump", "legs", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "legs", legsSize)
        .spriteSheet({1, 20}, {0, 5}).build());
    renderer->addToState("jump", "armBehind", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "body", bodySize)
        .spriteSheet({9, 4}, {2, 3}).build());
    renderer->addToState("jump", "body", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "body", bodySize)
        .spriteSheet({9, 4}, {1, 0}).build());
    renderer->addToState("jump", "head", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "head", headSize)
        .spriteSheet({1, 20}, {0, 0}).build());
    renderer->addToState("jump", "armFront", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "body", bodySize)
        .spriteSheet({9, 4}, {2, 1}).build());

    return player;
}
