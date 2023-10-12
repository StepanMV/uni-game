#pragma once

#include "renderer.h"
#include "physics.h"
#include "vec2.h"
#include <memory>

class Object {
public:
    Object() = default;
    Object(const Object& other);
    Object& operator=(const Object& other);

    bool checkCollision(const Object& other) const;
    bool MyCheckCollision(const Object& other) const;

    void calcHitbox();
    virtual bool isAlive() const = 0;


    virtual void update() = 0;
    virtual void render() = 0;
    Vec2 move();

    Vec2 getPos() const;
    Vec2 getSize() const;

protected:
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