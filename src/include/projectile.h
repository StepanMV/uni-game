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
        void setDirection(Vec2 direction);

        unsigned getId() const;

        virtual void breakObject() override;
        virtual bool isCollideable() const override;
        virtual bool isAlive() const override;

        virtual void update() override;
        void onCollision(std::shared_ptr<Tile> other) override;
        void onCollision(std::shared_ptr<Enemy> other) override;
        void onCollision(std::shared_ptr<Player> other) override;
        void onCollision(std::shared_ptr<Projectile> other) override;
        virtual void render() override;
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

        friend class ProjectileBuilder;
};

class ProjectileBuilder {
public:
    static ProjectileBuilder spawn(Vec2 pos, Vec2 size, unsigned _id);
    ProjectileBuilder& extra(double lifetime, unsigned _damage, bool _fromPlayer);

    std::shared_ptr<Projectile> build();

private:
    std::shared_ptr<Projectile> projectile;
};