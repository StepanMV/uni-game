#pragma once
#include "entity.h"

enum class EnemyType {
    ZOMBIE = 0,
    KingSlime,
    Slime,
    Eye,
    EyeOfCtulhu,
    Eow
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

        void readStats(std::string enemyName);

        unsigned getContactDamage() const;
    protected:
        Enemy() = default;
        EnemyType type;
        unsigned contactDamage = 1;
        std::shared_ptr<MyTransform> target;
};