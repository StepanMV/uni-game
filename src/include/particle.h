#pragma once
#include "object.h"

class Particle: public Object {
public: 
    static std::shared_ptr<Particle> create(unsigned id, Vec2 pos, Vec2 size);
    static std::shared_ptr<Particle> createFromObject(std::shared_ptr<Object> object);

    virtual void update() override;
    virtual void render() override;

    virtual void onCollision(std::shared_ptr<Tile> other) override {};
    virtual void onCollision(std::shared_ptr<Enemy> other) override {};
    virtual void onCollision(std::shared_ptr<Projectile> other) override {};
    virtual void onCollision(std::shared_ptr<Player> other) override {};
    virtual bool isCollideable() const override { return false; };
private:
    Particle();
    Particle(const Object& other);

    friend class UI;
    friend class ScrollMenu;
};