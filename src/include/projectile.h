#pragma once
#include "object.h"
#include "timer.h"

class Tile;

class Projectile: public Object {
    public:
        Projectile();
        Projectile(const Projectile& other);
        Projectile& operator=(const Projectile& other);
        Projectile(unsigned _id, unsigned _damage, const bool _fromPlayer);

        void setDirection(Vec2 direction);

        unsigned getId() const;

        void breakProjectile();
        virtual bool isAlive() const override;

        virtual void update() override;
        void onCollision(Tile& other);
        virtual void render() override;
        void spawn(Vec2 pos, Vec2 size, double lifetime);
        void moveStraight();
        void moveHoming();
        void bounce();
        void redirect();
        void hit();
        void falling();

    private:
        std::shared_ptr<Timer> timer;
        unsigned damage;
        bool fromPlayer;
        unsigned id;
};