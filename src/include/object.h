#pragma once

#include "renderer.h"
#include "physics.h"
#include "level.h"
#include "vec2.h"
#include <memory>

class Object {
public:

    bool checkCollision(const std::shared_ptr<Object> other) const;
    bool MyCheckCollision(const std::shared_ptr<Object> other) const;
    virtual void onCollision(std::shared_ptr<Tile> other) = 0;
    virtual void onCollision(std::shared_ptr<Player> other) = 0;
    virtual void onCollision(std::shared_ptr<Enemy> other) = 0;
    virtual void onCollision(std::shared_ptr<Projectile> other) = 0;
    //virtual void onCollision(const std::shared_ptr<Object> other) = 0;

    void calcHitbox();
    virtual bool isAlive() const = 0;
    virtual bool isCollideable() const = 0;
    virtual void breakObject() = 0;


    virtual void update() = 0;
    virtual void render() = 0;
    void move();
    Vec2 getSpeed();

    Vec2 getPos() const;
    Vec2 getSize() const;

protected:
    Object() = default;
    Object(const Object& other);
    Object& operator=(const Object& other);

    Vec2 pos, size;
    std::vector<Vec2> hitbox;
    float angle = 0;

    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<Physics> physics;
};

struct Circle {
    Vec2 pos;
    float radius;
};