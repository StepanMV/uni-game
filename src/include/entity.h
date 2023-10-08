#pragma once

#include "object.h"
#include "vec2.h"

class Tile;
class Projectile;

class Entity: public Object {
    public:
        Entity() = default;
        Entity(const Entity& other);
        Entity& operator=(const Entity& other);
        virtual void onCollision(Tile& other) = 0;
        virtual void onCollision(Entity& other) = 0;

        virtual bool isAlive() const override;
        
        bool getAttacking() const;
        virtual Projectile getProjectile() const = 0;
        
        /*
        virtual void move();
        virtual void jump();
        virtual void dash();
        virtual void falling();
        virtual void takeDamage();
        */

    protected:
        bool isAttacking = false;
        unsigned max_health = 100;
        unsigned health = max_health;
};