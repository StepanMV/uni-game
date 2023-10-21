#pragma once

#include "entity.h"
#include "timer.h"
#include "projectile.h"
#include "weapon.h"

class Player: public Entity {
    public:
        virtual void update() override;
        virtual void render() override;

        void moveEditor();

        void attack() override;

        virtual void onCollision(std::shared_ptr<Tile> other) override;
        virtual void onCollision(std::shared_ptr<Enemy> other) override;
        virtual void onCollision(std::shared_ptr<Player> other) override;
        virtual void onCollision(std::shared_ptr<Projectile> other) override;

        virtual bool isCollideable() const override;

        void onBoard();
        /*
        void moveRight();
        void moveLeft();
        virtual void jump() override;
        virtual void dash() override;
        virtual void falling() override;
        virtual void takeDamage() override;
        */

       static std::map<unsigned int, std::shared_ptr<Player>> players;

    private:
        bool facingLeft = false;
        bool skipPlatform = false;
        bool isWeapon = false;
        std::shared_ptr<Timer> projTimer = Timer::getInstance(0.1);
        std::shared_ptr<Timer> platformTimer = Timer::getInstance(0.15);
        std::shared_ptr<Weapon> weapon;
        double startY = transform->pos.y;
        friend class PlayerBuilder;
};

class PlayerBuilder {
public:
    static PlayerBuilder spawn(unsigned id, Vec2 pos, Vec2 size);
    PlayerBuilder& setMaxSpeeds(double maxMoveSpeed, double maxFallSpeed, double maxFlySpeed);
    PlayerBuilder& setForces(double friction, double gravity);

    std::shared_ptr<Player> build();

private:
    std::shared_ptr<Player> player;
};