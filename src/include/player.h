#pragma once

#include "entity.h"
#include "timer.h"
#include "projectile.h"

class Player: public Entity {
    public:
        Player();

        virtual void update() override;
        virtual void render() override;

        Projectile getProjectile() const override;
        Projectile getWeapon() const;

        virtual void onCollision(Tile& other) override;
        virtual void onCollision(Entity& other) override;
        void onBoard();
        /*
        void moveRight();
        void moveLeft();
        virtual void jump() override;
        virtual void dash() override;
        virtual void falling() override;
        virtual void takeDamage() override;
        */

    private:
        bool facingLeft = false;
        bool isWeapon = false;
        Projectile weapon;
        std::shared_ptr<Timer> projTimer = Timer::getInstance(0.1/*, [&](){isAttacking = true;}*/);
        double startY = pos.y;
        friend class PlayerBuilder;
};

class PlayerBuilder {
public:
    static PlayerBuilder spawn(Vec2 pos, Vec2 size);
    PlayerBuilder& setMaxSpeeds(double maxMoveSpeed, double maxFallSpeed, double maxFlySpeed);
    PlayerBuilder& setForces(double friction, double gravity);
    PlayerBuilder& setHeadTexture(const std::string& texturePath);
    PlayerBuilder& setLegsTexture(const std::string& texturePath);
    PlayerBuilder& setBodyTexture(const std::string& texturePath);

    Player build();

private:
    std::string headTexturePath, legsTexturePath, bodyTexturePath;
    Player player;
};