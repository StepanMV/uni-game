#pragma once
#include "object.h"

class Particle: public Object {
public: 
    virtual void update() override {};
    virtual void render() override {};

    virtual void onCollision(std::shared_ptr<Tile> other) override {};
    virtual void onCollision(std::shared_ptr<Enemy> other) override {};
    virtual void onCollision(std::shared_ptr<Projectile> other) override {};
    virtual void onCollision(std::shared_ptr<Player> other) override {};
private:
    Particle();
    double time;
};