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
    void setState(std::string state);

    static std::vector<std::shared_ptr<Object>> objects;
    static std::vector<std::vector<std::shared_ptr<Tile>>> tiles;
    
protected:
    friend class UIBuilder;

    Object() = default;
    Object(const Object& other);
    Object& operator=(const Object& other);

    unsigned int id = 0;

    bool tileCollide = true, objectCollide = true;

    std::shared_ptr<MyTransform> transform = std::make_shared<MyTransform>();
    std::shared_ptr<Collider> collider;
    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<Physics> physics;
};