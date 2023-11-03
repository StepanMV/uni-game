#pragma once

#include "entity.h"
#include "timer.h"
#include "projectile.h"
#include "weapon.h"

class Player: public Entity {
    public:
        virtual void update() override;
        virtual void render() override;

        virtual void onCollision(std::shared_ptr<Enemy> other) override;
        virtual void onCollision(std::shared_ptr<Player> other) override {};
        virtual void onCollision(std::shared_ptr<Projectile> other) override;

        virtual bool isCollideable() const override;

    private:
        virtual void readStats(std::string playerName);
        void moveEditor();
        void attack() override;
        std::shared_ptr<Weapon> weapon;
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