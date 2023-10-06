#pragma once

#include "renderer.h"
#include "physics.h"
#include "vec2.h"

class Object {
public:
    Object();
    Object(const Object& other);
    Object& operator=(const Object& other);

    bool checkCollision(const Object& other) const;
    Rectangle getCollisionBox(const Object& other) const;


    virtual void update() = 0;
    virtual void render() = 0;
    void move();

    Vec2 getPos() const;
    Vec2 getSize() const;

protected:
    Vec2 pos, size;

    Renderer renderer;
    Physics physics;
};