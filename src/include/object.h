#pragma once

#include "renderer.h"
#include "physics.h"
#include "level.h"
#include "collider.h"
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

    bool tileCollide = true, objectCollide = true;

    std::shared_ptr<Collider> collider = std::make_shared<Collider>(&pos, &size, &angle);
    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<Physics> physics;
};