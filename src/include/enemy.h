#pragma once
#include "entity.h"

enum class EnemyType {
    ZOMBIE = 0,
    KingSlime,
    EYE
};

class Enemy: public Entity {
    public:
        virtual void onCollision(std::shared_ptr<Enemy> other) override {};
        virtual void onCollision(std::shared_ptr<Projectile> other) override;
        virtual void onCollision(std::shared_ptr<Player> other) override {};
        virtual bool isCollideable() const override;

        virtual void update() override;
        virtual void render() override;
        virtual void attack() override {};

        unsigned getContactDamage() const;
    protected:
        Enemy() = default;
        EnemyType type;
        unsigned contactDamage = 1;
        //slime
        //double stayTime = 2;
        //std::shared_ptr<Timer> stayTimer = Timer::getInstance(stayTime + jumpTime);
        //
        std::shared_ptr<MyTransform> target;
        friend class EnemyBuilder;
};

class EnemyBuilder {
    public:
        // static EnemyBuilder spawn(unsigned id, EnemyType type, Vec2 pos, Vec2 size);
        static std::shared_ptr<Enemy> spawn(EnemyType type, Vec2 pos, std::shared_ptr<MyTransform> target);
        EnemyBuilder& setMaxSpeeds(double maxMoveSpeed, double maxFallSpeed, double maxFlySpeed);
        EnemyBuilder& setForces(double friction, double gravity);
        EnemyBuilder& setTarget(std::shared_ptr<MyTransform> target);

        std::shared_ptr<Enemy> build();
    private:
        std::shared_ptr<Enemy> enemy;
};