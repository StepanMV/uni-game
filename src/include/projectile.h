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
    virtual void onCollision(std::shared_ptr<Projectile> other) override;
    virtual void render() override;

    bool getFromPlayer() const;
    unsigned getDamage() const;

    void moveStraight();
    void moveHoming();
    void bounce();
    void redirect();
    void hit();
    void falling();

protected:
    Projectile() = default;

    std::shared_ptr<Timer> timer;
    unsigned damage;
    bool fromPlayer;

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