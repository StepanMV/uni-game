#pragma once

#include "renderer.h"
#include "physics.h"
#include "vec2.h"
#include <memory>

class Object {
public:

    bool checkCollision(const Object& other) const;
    Rectangle getCollisionBox(const Object& other) const;


    virtual void update() = 0;
    virtual void render() = 0;
    Vec2 move();

    Vec2 getPos() const;
    Vec2 getSize() const;

protected:
    Vec2 pos, size;

    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<Physics> physics;
};