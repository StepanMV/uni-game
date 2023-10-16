#include "player.h"
#include "tile.h"
#include "level.h"
#include "keyboard.h"

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
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isAttacking = true;
    }
    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        isAttacking = false;
    }
    if (Keyboard::isKeyDown(KEY_S)) {
        if(physics->onGround) {
            skipPlatform = true;
        }
        platformTimer->reset();
    }
    if(platformTimer->isDone()) {
        skipPlatform = false;
    }
    calcHitbox();
    physics->onGround = false;
    attack();
}

void Player::moveEditor() {
    onBoard();
    physics->accel = Vec2(0, 0);
    if (IsKeyDown(KEY_A)) {
        physics->accel += Vec2(-1.5, 0);
    }
    if (IsKeyDown(KEY_D)) {
        physics->accel += Vec2(1.5, 0);
    }
    if (IsKeyDown(KEY_W)) {
        physics->accel += Vec2(0, -1.5);
    }
    if (IsKeyDown(KEY_S)) {
        physics->accel += Vec2(0, 1.5);
    }
}

void Player::attack() {
    if(isAttacking) {
        Vector2 mousePos = GetScreenToWorld2D({(float) GetMouseX(), (float) GetMouseY()}, Level::camera);
        Vec2 worldMP = Vec2(mousePos.x, mousePos.y);
        Vec2 spawnPos = worldMP - pos;
        spawnPos.normalize();
        spawnPos *= size.x / 2;
        spawnPos += pos;
        if(worldMP.x < pos.x) facingLeft = true;
        else facingLeft = false;
        auto proj = ProjectileBuilder::spawn(spawnPos, Vec2(22, 24), 1).extra(10, 1, true).build();
        proj->setDirection(worldMP);
    }
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

void Player::onCollision(std::shared_ptr<Tile> other) {
    if(other->isPlatform && skipPlatform) {
        return;
    }
    if(!other->isUp && (physics->speed.y > 0) && (pos.y + size.y / 2 < other->getPos().y + other->getSize().y / 2)) {
        if(other->getPos().x + other->getSize().x / 2 - pos.x + size.x / 2 <= 2) {
            pos.x = other->getPos().x + other->getSize().x / 2 + size.x / 2 - 1;
        }
        else if(pos.x + size.x / 2 - other->getPos().x + other->getSize().x / 2 <= 2) {
            pos.x = other->getPos().x - other->getSize().x / 2 - size.x / 2 + 1;
        }
        else {
            physics->speed.y = 0;
            physics->onGround = true;
            pos.y = other->getPos().y - other->getSize().y / 2 - size.y / 2 + 1;
        }
        calcHitbox();
    }
    if(other->isPlatform) return;
    if(!other->isDown && physics->speed.y < 0 && pos.y - size.y / 2 > other->getPos().y - other->getSize().y / 2){
        if(other->getPos().x + other->getSize().x / 2 - pos.x + size.x / 2 <= 2) {
            pos.x = other->getPos().x + other->getSize().x / 2 + size.x / 2 - 1;
        }
        else if(pos.x + size.x / 2 - other->getPos().x + other->getSize().x / 2 <= 2) {
            pos.x = other->getPos().x - other->getSize().x / 2 - size.x / 2 + 1;
        }
        else {
            physics->speed.y = 0;
            physics->jumping = false;
            pos.y = other->getPos().y + other->getSize().y / 2 + size.y / 2 - 1;
        }
        calcHitbox();
    }
    if(!other->isLeft && (physics->speed.x > 0) && (pos.x + size.x / 2 < other->getPos().x + other->getSize().x / 2)) {
        if((other->canClimbLeft) && (pos.y <= other->getPos().y - other->getSize().y / 2)) {
            pos.y = other->getPos().y - other->getSize().y / 2 - size.y / 2 + 1;
        }
        else if(other->getPos().y + other->getSize().y / 2 - pos.y + size.y / 2 <= 2) {
            pos.y = other->getPos().y + other->getSize().y / 2 + size.y / 2 - 1;
        }
        else {
            physics->speed.x = 0;
            pos.x = other->getPos().x - other->getSize().x / 2 - size.x / 2 + 1;
        }
        calcHitbox();
    }
    if(!other->isRight && (physics->speed.x < 0) && (pos.x - size.x / 2 > other->getPos().x - other->getSize().x / 2)) {
        if((other->canClimbRight) && (pos.y <= other->getPos().y - other->getSize().y / 2)) {
            pos.y = other->getPos().y - other->getSize().y / 2 - size.y / 2 + 1;
        }
        else if(other->getPos().y + other->getSize().y / 2 - pos.y + size.y / 2 <= 2) {
            pos.y = other->getPos().y + other->getSize().y / 2 + size.y / 2 - 1;
        }
        else {
            physics->speed.x = 0;
            pos.x = other->getPos().x + other->getSize().x / 2 + size.x / 2 - 1;
        }
        calcHitbox();
    }
}

void Player::onCollision(std::shared_ptr<Enemy> other) {
    
}

void Player::onCollision(std::shared_ptr<Projectile> other) {
    
}

void Player::onCollision(std::shared_ptr<Player> other) {
    
}

bool Player::isCollideable() const
{
    return true;
}

void Player::breakObject() {
    //?
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

PlayerBuilder &PlayerBuilder::setTextureID(const int textureID) {
    this->textureID = textureID;
    return *this;
}

Player PlayerBuilder::build()
{
    if (textureID == 0) return player;
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(player.renderer);

    Vec2 legsSize = renderer->loadTexture("legs", "resources/textures/Armor_Legs_" + std::to_string(textureID) + ".png");
    Vec2 headSize = renderer->loadTexture("head", "resources/textures/Armor_Head_" + std::to_string(textureID) + ".png");
    Vec2 bodySize = renderer->loadTexture("body", "resources/textures/Armor_" + std::to_string(textureID) + ".png");


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
