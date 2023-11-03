#pragma once
#include "entity.h"

class Enemy: public Entity {
public:
    virtual void onCollision(std::shared_ptr<Enemy> other) override {};
    virtual void onCollision(std::shared_ptr<Projectile> other) override;
    virtual void onCollision(std::shared_ptr<Player> other) override {};
    virtual bool isCollideable() const override;
    virtual void update() = 0;
    virtual void render() = 0;

    unsigned getContactDamage() const;
protected:
    virtual void readStats(std::string enemyName);
    virtual void attack() override {};
    Enemy() = default;
    unsigned contactDamage = 1;
    // std::shared_ptr<MyTransform> target;
    std::shared_ptr<Player> target;
};