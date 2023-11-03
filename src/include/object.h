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

    static void updateEnemies();
    static void renderEnemies();

    static void updateParticles();
    static void renderParticles();

    static void updateProjectiles();
    static void renderProjectiles();
    
    static void updatePlayer();
    static void renderPlayer();

    static void clearObjects();
    virtual void destroy();

    virtual void onCollision(std::shared_ptr<Tile> other) = 0;
    virtual void onCollision(std::shared_ptr<Player> other) = 0;
    virtual void onCollision(std::shared_ptr<Enemy> other) = 0;
    virtual void onCollision(std::shared_ptr<Projectile> other) = 0;
    
    bool isAlive() const;
    virtual bool isCollideable() const = 0;


    virtual void update() = 0;
    virtual void render() = 0;

    unsigned getId() const;
    Vec2 getSpeed() const;
    Vec2 getPos() const;
    Vec2 getSize() const;
    float getAngle() const;
    Vec2 getCenterOffset() const;
    void setState(std::string state);

    static void addProjectile(std::shared_ptr<Projectile> projectile);
    static void addEnemy(std::shared_ptr<Enemy> enemy);
    static void addPlayer(std::shared_ptr<Player> player);
    static void addParticle(std::shared_ptr<Particle> particle);
    
protected:
    friend class UIBuilder;
    friend class Level;
    friend class Particle;
    static std::vector<std::shared_ptr<Projectile>> projectiles;
    static std::vector<std::shared_ptr<Enemy>> enemies;
    static std::vector<std::shared_ptr<Particle>> particles;
    static std::vector<std::vector<std::shared_ptr<Tile>>> tiles;
    static std::shared_ptr<Player> player;
    Object() = default;
    Object(const Object& other);
    Object& operator=(const Object& other);

    unsigned int id = 0;

    bool tileCollide = true, objectCollide = true;
    std::string destroySound = "";

    std::shared_ptr<MyTransform> transform = std::make_shared<MyTransform>();
    std::shared_ptr<Collider> collider;
    std::shared_ptr<Renderer> renderer;
    std::shared_ptr<Physics> physics;
};