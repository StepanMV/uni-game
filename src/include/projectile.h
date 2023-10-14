#pragma once
#include "object.h"
#include "timer.h"

class Tile;

class Projectile: public Object {
    public:
        template<class... Args>
        static std::shared_ptr<Projectile> createProjectile(Args&&... args) {
            std::shared_ptr<Projectile> proj = std::shared_ptr<Projectile>(new Projectile(args...));
            Level::addObject(proj);
            return proj;
        }
        Projectile& setDirection(Vec2 direction);

        unsigned getId() const;

        virtual void breakObject() override;
        virtual bool isCollideable() const override;
        virtual bool isAlive() const override;

        virtual void update() override;
        void onCollision(std::shared_ptr<Tile> other) override;
        void onCollision(std::shared_ptr<Entity> other) override;
        void onCollision(std::shared_ptr<Projectile> other) override;
        virtual void render() override;
        Projectile& spawn(Vec2 pos, Vec2 size, double lifetime);
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