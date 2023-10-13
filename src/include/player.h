#pragma once

#include "entity.h"
#include "timer.h"


class Player: public Entity {
    public:
        virtual void update() override;
        virtual void render() override;

        void goDownEditor();

        void getProjectile() const override;

        virtual void onCollision(std::shared_ptr<Tile> other) override;
        virtual void onCollision(std::shared_ptr<Entity> other) override;
        virtual void onCollision(std::shared_ptr<Projectile> other) override;

        virtual bool isCollideable() const override;
        virtual void breakObject() override;

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
        Player() = default;

        bool facingLeft = false;
        bool skipPlatform = false;
        std::shared_ptr<Timer> projTimer = Timer::getInstance(0.1);
        std::shared_ptr<Timer> platformTimer = Timer::getInstance(0.15);
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