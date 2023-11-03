#include "player.h"
#include "tile.h"
#include "level.h"
#include "controls.h"
#include "game.h"
#include "enemy.h"
#include "bosses.h"

void Player::update() {
    if (id == 1) {
        moveEditor();
        return;
    }
    physics->accel = Vec2(0, 0);
    if(Controls::isKeyDoublePressed(KEY_D)) {
        dash(Vec2(1, 0), 30);
    }
    if(Controls::isKeyDoublePressed(KEY_A)) {
        dash(Vec2(-1, 0), 30);
    }
    if (IsKeyDown(KEY_A)) {
        move(Vec2(-1.5, 0));
    }
    if (IsKeyDown(KEY_D)) {
        move(Vec2(1.5, 0));
    }
    if(Controls::isKeyPressed(KEY_SPACE)) {
        jump();
    }
    if(Controls::isKeyDown(KEY_SPACE)) {
        fly();
        plane();
    }
    else {
        falling();
    }
    if(onGround) {
        currentFlightTime = maxFlightTime;
    }
    if(Controls::isMouseDown(MOUSE_LEFT_BUTTON)) {
        if(!isAttacking || !weapon->isAlive()) {
            weapon = Weapon::spawn(weaponId, transform, true);
        }
        isAttacking = true;
    }
    if(Controls::isMouseReleased(MOUSE_LEFT_BUTTON)) {
        if(!weapon->isAlive()) {
            isAttacking = false;
        }
    }
    if (Controls::isKeyDown(KEY_S)) {
        if(onGround) {
            skipPlatform = true;
        }
        platformTimer->reset();
    }
    if(platformTimer->isDone()) {
        skipPlatform = false;
    }

    if (abs(physics->speed.x) > 7 && onGround) {
        if(!runSoundTimer) runSoundTimer = Timer::getInstance(0.15, true, [](){Audio::playSound("Run");});
    } else {
        runSoundTimer = nullptr;
    }

    collider->calcHitbox();
    onGround = false;
    onBoard();

    if(weapon) weapon->setLeftSide(facingLeft);

    // if(Controls::isKeyPressed(KEY_E)) {
    //     weaponId++;
    // }
    // if(Controls::isKeyPressed(KEY_Q)) {
    //     weaponId--;
    // }

    // if(weaponId < 1) weaponId = 1;
    // if(weaponId > 12) weaponId = 12;
}

void Player::readStats(std::string playerName) {
    
}

void Player::moveEditor()
{
    onBoard();
    physics->accel = Vec2(0, 0);
    if (Controls::isKeyDown(KEY_A)) {
        physics->accel += Vec2(-1, 0);
    }
    if (Controls::isKeyDown(KEY_D)) {
        physics->accel += Vec2(1, 0);
    }
    if (Controls::isKeyDown(KEY_W)) {
        physics->accel += Vec2(0, -1);
    }
    if (Controls::isKeyDown(KEY_S)) {
        physics->accel += Vec2(0, 1);
    }
}

void Player::render() {
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(this->renderer);
    renderer->setState("idle");
    if (physics->speed.x > 0) {
        facingLeft = false;
        renderer->setState("moving");
        renderer->setAnimationSpeed(10 * physics->speed.x / physics->maxMoveSpeed);
    } else if (physics->speed.x < 0) {
        facingLeft = true;
        renderer->setState("moving");
        renderer->setAnimationSpeed(10 * -physics->speed.x / physics->maxMoveSpeed);
    }

    if (isFlying) {
        if (physics->speed.y < 0) {
            renderer->setState("flying");
        } else if (physics->speed.y > 0) {
            renderer->setState("slowFalling");
        }
    } else if (!onGround) {
        renderer->setState("inAir");
    }
    renderer->setFlipped(facingLeft);
}

void Player::onCollision(std::shared_ptr<Enemy> other) {
    if(damageTimer->isDone()) {
        takeDamage(other->getContactDamage());
        takeKnockback(other->getPos().x);
        damageTimer->reset();
    }
}

void Player::onCollision(std::shared_ptr<Projectile> other) {
    if(!other->getFromPlayer()) {
        if(damageTimer->isDone()) {
            takeDamage(other->getDamage());
            if(other->getDamage()) takeKnockback(other->getCenterOffset().x + other->getPos().x);
            damageTimer->reset();
        }
    }
}

bool Player::isCollideable() const {
    return true;
}

PlayerBuilder PlayerBuilder::spawn(unsigned id, Vec2 pos, Vec2 size) {
    PlayerBuilder builder;
    builder.player = std::make_shared<Player>();
    builder.player->id = id;
    builder.player->weaponId = id;
    builder.player->transform->pos = pos;
    builder.player->transform->size = size;
    builder.player->renderer = std::make_shared<CoolRenderer>(builder.player->transform);
    builder.player->physics = std::make_shared<Physics>();
    builder.player->destroySound = "Player_Killed";
    builder.player->damageSound = "Player_Hit_0";
    return builder;
}

PlayerBuilder &PlayerBuilder::setMaxSpeeds(double maxMoveSpeed, double maxFallSpeed, double maxFlySpeed) {
    player->physics->maxMoveSpeed = maxMoveSpeed;
    player->physics->maxFallSpeed = maxFallSpeed;
    player->physics->maxFlySpeed = maxFlySpeed;
    return *this;
}

PlayerBuilder &PlayerBuilder::setForces(double friction, double gravity) {
    player->physics->friction = friction;
    player->physics->gravity = gravity;
    return *this;
}
std::shared_ptr<Player> PlayerBuilder::build()
{
    if (player->id == 0) return player;

    Object::addPlayer(player);

    if (player->id == 1) return player;

    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(player->renderer);
    Vec2 size = player->transform->size;

    Vec2 legsSize = renderer->loadTexture("legs", "resources/textures/Player_Legs_" + std::to_string(player->id) + ".png");
    Vec2 headSize = renderer->loadTexture("head", "resources/textures/Player_Head_" + std::to_string(player->id) + ".png");
    Vec2 bodySize = renderer->loadTexture("body", "resources/textures/Player_Body_" + std::to_string(player->id) + ".png");
    Vec2 wingsSize = renderer->loadTexture("wings", "resources/textures/Player_Wings_" + std::to_string(player->id) + ".png");

    player->damageTimer = Timer::getInstance(0.5);
    player->knockbackResist = false;
    player->canClimb = true;

    renderer->addToState("idle", "wings", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "wings", wingsSize)
        .spriteSheet({1, 4}, {0, 0}).setExtra(false, 0, 1.75).setDestOffset({-0.125f * size.x, 0.125f * size.y}).keepProportions().build());
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

    renderer->addToState("moving", "wings", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "wings", wingsSize)
        .spriteSheet({1, 4}, {0, 0}).setExtra(false, 0, 1.75).setDestOffset({-0.125f * size.x, 0.125f * size.y}).keepProportions().build());
    renderer->addToState("moving", "head", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "head", headSize)
        .spriteSheet({1, 20}, {0, 0}).build());
    renderer->addToState("moving", "legs", TextureDataBuilder::init(TextureType::ANIMATION, "legs", legsSize)
        .animation({1, 20}, {0, 6}, {0, 19}, 14).build());
    renderer->addToState("moving", "armBehind", TextureDataBuilder::init(TextureType::ANIMATION, "body", bodySize)
        .animation({9, 4}, {3, 3}, {6, 3}, 4).build());
    renderer->addToState("moving", "body", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "body", bodySize)
        .spriteSheet({9, 4}, {0, 0}).build());
    renderer->addToState("moving", "armFront", TextureDataBuilder::init(TextureType::ANIMATION, "body", bodySize)
        .animation({9, 4}, {3, 1}, {6, 1}, 4).build());
    renderer->addToState("moving", "shoulder", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "body", bodySize)
        .spriteSheet({9, 4}, {0, 1}).build());

    renderer->addToState("inAir", "wings", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "wings", wingsSize)
        .spriteSheet({1, 4}, {0, 1}).setExtra(false, 0, 1.75).setDestOffset({-0.125f * size.x, 0.125f * size.y}).keepProportions().build());
    renderer->addToState("inAir", "legs", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "legs", legsSize)
        .spriteSheet({1, 20}, {0, 5}).build());
    renderer->addToState("inAir", "armBehind", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "body", bodySize)
        .spriteSheet({9, 4}, {2, 3}).build());
    renderer->addToState("inAir", "body", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "body", bodySize)
        .spriteSheet({9, 4}, {1, 0}).build());
    renderer->addToState("inAir", "head", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "head", headSize)
        .spriteSheet({1, 20}, {0, 0}).build());
    renderer->addToState("inAir", "armFront", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "body", bodySize)
        .spriteSheet({9, 4}, {2, 1}).build());

    renderer->addToState("flying", "wings", TextureDataBuilder::init(TextureType::ANIMATION, "wings", wingsSize)
        .animation({1, 4}, {0, 0}, {0, 3}, 8).setExtra(false, 0, 1.75).setDestOffset({-0.125f * size.x, 0.125f * size.y}).keepProportions().build());
    renderer->addToState("flying", "legs", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "legs", legsSize)
        .spriteSheet({1, 20}, {0, 5}).build());
    renderer->addToState("flying", "armBehind", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "body", bodySize)
        .spriteSheet({9, 4}, {3, 3}).build());
    renderer->addToState("flying", "body", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "body", bodySize)
        .spriteSheet({9, 4}, {1, 0}).build());
    renderer->addToState("flying", "head", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "head", headSize)
        .spriteSheet({1, 20}, {0, 0}).build());
    renderer->addToState("flying", "armFront", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "body", bodySize)
        .spriteSheet({9, 4}, {3, 1}).build());
    renderer->addToState("flying", "shoulder", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "body", bodySize)
        .spriteSheet({9, 4}, {0, 1}).build());

    renderer->addToState("slowFalling", "wings", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "wings", wingsSize)
        .spriteSheet({1, 4}, {0, 2}).setExtra(false, 0, 1.75).setDestOffset({-0.125f * size.x, 0.125f * size.y}).keepProportions().build());
    renderer->addToState("slowFalling", "legs", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "legs", legsSize)
        .spriteSheet({1, 20}, {0, 5}).build());
    renderer->addToState("slowFalling", "armBehind", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "body", bodySize)
        .spriteSheet({9, 4}, {3, 3}).build());
    renderer->addToState("slowFalling", "body", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "body", bodySize)
        .spriteSheet({9, 4}, {1, 0}).build());
    renderer->addToState("slowFalling", "head", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "head", headSize)
        .spriteSheet({1, 20}, {0, 0}).build());
    renderer->addToState("slowFalling", "armFront", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "body", bodySize)
        .spriteSheet({9, 4}, {3, 1}).build());
    renderer->addToState("slowFalling", "shoulder", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "body", bodySize)
        .spriteSheet({9, 4}, {0, 1}).build());
    renderer->setState("idle");

    return player;
}
