#pragma once
#include "object.h"
#include "timer.h"

class Tile;

class Projectile: public Object {
    public:
        void setDirection(Vec2 direction);

        void setId(unsigned id);

        virtual bool isCollideable() const override;

        virtual void update() override;
        virtual void onCollision(std::shared_ptr<Tile> other) override;
        virtual void onCollision(std::shared_ptr<Enemy> other) override;
        virtual void onCollision(std::shared_ptr<Player> other) override;
        virtual void onCollision(std::shared_ptr<Projectile> other) override {};
        virtual void render() override {};

        bool getFromPlayer() const;
        unsigned getDamage() const;

        static std::shared_ptr<Projectile> spawn(unsigned id, Vec2 pos, bool fromPlayer, unsigned weaponDamage = 0);

    protected:
        Projectile() = default;
        unsigned damage;
        bool fromPlayer;
        int hitCount = 0;
        std::shared_ptr<Timer> hitTimer = Timer::getInstance(0.1);
        std::shared_ptr<Timer> timer;
};