#pragma once
#include "object.h"
#include "timer.h"

class Tile;

class Projectile: public Object {
    public:
        template<class... Args>
        static void createProjectile(Args&&... args) {
            Projectile* proj = new Projectile(args...);
            proj->spawn(Vec2(0, 0), Vec2(22, 24), 10);
            //Level::addObject(std::shared_ptr<Projectile>(new Projectile(args...)));
            Level::addObject(std::shared_ptr<Projectile>(proj));
        }
        void setDirection(Vec2 direction);

        unsigned getId() const;

        virtual void breakObject() override;
        virtual bool isCollideable() const override;
        virtual bool isAlive() const override;

        virtual void update() override;
        void onCollision(std::shared_ptr<Tile> other) override;
        void onCollision(std::shared_ptr<Entity> other) override;
        void onCollision(std::shared_ptr<Projectile> other) override;
        virtual void render() override;
        void spawn(Vec2 pos, Vec2 size, double lifetime);
        void moveStraight();
        void moveHoming();
        void bounce();
        void redirect();
        void hit();
        void falling();

    private:
        Projectile();
        Projectile(const Projectile& other);
        Projectile& operator=(const Projectile& other);
        Projectile(unsigned _id, unsigned _damage, const bool _fromPlayer);

        std::shared_ptr<Timer> timer;
        unsigned damage;
        bool fromPlayer;
        unsigned id;
};