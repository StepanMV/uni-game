#pragma once

#include "renderer.h"
#include "physics.h"
#include "vec2.h"

class Object {
public:
    bool checkCollision(const Object& other) const;
    bool MyCheckCollision(const Object& other) const;
    Rectangle getCollisionBox(const Object& other) const;

    void calcHitbox();
    virtual bool isAlive() const = 0;


    virtual void update() = 0;
    virtual void render() = 0;
    void move();

    Vec2 getPos() const;
    Vec2 getSize() const;

protected:
    Vec2 pos, size;
    std::vector<Vec2> hitbox;
    float angle = 0;

    Renderer renderer;
    Physics physics;
};

struct Circle {
    Vec2 pos;
    float radius;
};