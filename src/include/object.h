#pragma once

#include "renderer.h"
#include "physics.h"
#include "level.h"
#include "vec2.h"
#include <memory>

class Object {
public:
    static void updateAll();
    static void renderAll();
    void destroy();

    bool checkCollision(const std::shared_ptr<Object> other) const;
    bool MyCheckCollision(const std::shared_ptr<Object> other) const;
    virtual void onCollision(std::shared_ptr<Tile> other) = 0;
    virtual void onCollision(std::shared_ptr<Player> other) = 0;
    virtual void onCollision(std::shared_ptr<Enemy> other) = 0;
    virtual void onCollision(std::shared_ptr<Projectile> other) = 0;
    //virtual void onCollision(const std::shared_ptr<Object> other) = 0;

    void calcHitbox();
    void setCenterOffset(Vec2 offset);
    bool isAlive() const;
    virtual bool isCollideable() const = 0;


    virtual void update() = 0;
    virtual void render() = 0;

    unsigned getId() const;
    Vec2 getSpeed();
    Vec2 getPos() const;
    Vec2 getSize() const;
    float getAngle() const;

    static std::vector<std::shared_ptr<Object>> objects;
    static std::vector<std::vector<std::shared_ptr<Tile>>> tiles;
    
protected:

    Object() = default;
    Object(const Object& other);
    Object& operator=(const Object& other);

    unsigned int id = 0;
    Vec2 pos = Vec2(0, 0);
    Vec2 size = Vec2(0, 0);
    float angle = 0;

    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<Physics> physics;

    std::vector<Vec2> hitbox = std::vector<Vec2>(4);
    Vec2 startCenter = Vec2(0, 0);
    Vec2 centerOffset = Vec2(0, 0);
    std::vector<Vec2> startHitbox = {
        Vec2(+size.x / 2, +size.y / 2),
        Vec2(+size.x / 2, -size.y / 2),
        Vec2(-size.x / 2, -size.y / 2),
        Vec2(-size.x / 2, +size.y / 2)
    };
};

struct Circle {
    Vec2 pos;
    float radius;
};