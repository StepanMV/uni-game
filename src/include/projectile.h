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
        virtual void render() override;

        bool getFromPlayer() const;
        unsigned getDamage() const;

        static std::shared_ptr<Projectile> spawn(unsigned id, Vec2 pos, bool fromPlayer, unsigned weaponDamage = 0);

        void moveStraight();
        void moveHoming();
        void bounce();
        void redirect();
        void hit();
        void falling();

    protected:
        Projectile() = default;
        unsigned damage;
        bool fromPlayer;
        bool toLeft = false;
        std::shared_ptr<Timer> timer;
};