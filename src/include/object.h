#pragma once

#include "renderer.h"
#include "physics.h"
#include "vec2.h"
#include <memory>

class Object {
public:

    bool checkCollision(const Object& other) const;
    bool MyCheckCollision(const Object& other) const;
    Rectangle getCollisionBox(const Object& other) const;

    void calcHitbox();
    void setCenterOffset(Vec2 offset);
    virtual bool isAlive() const = 0;


    virtual void update() = 0;
    virtual void render() = 0;
    Vec2 move();

    Vec2 getPos() const;
    Vec2 getSize() const;

protected:
    Vec2 pos, size;
    std::vector<Vec2> hitbox = std::vector<Vec2>(4);
    
    std::vector<Vec2> startHitbox = {Vec2(+size.x / 2, +size.y / 2),
                                    Vec2(+size.x / 2, -size.y / 2),
                                    Vec2(-size.x / 2, -size.y / 2),
                                    Vec2(-size.x / 2, +size.y / 2)};
    Vec2 startCenter = Vec2(0, 0);
    
    
    Vec2 centerOffset = Vec2(0, 0);
    float angle = 0;

    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<Physics> physics;
};

struct Circle {
    Vec2 pos;
    float radius;
};