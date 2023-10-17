#pragma once

#include "object.h"
#include "vec2.h"

class Tile;
class Projectile;

class Entity: public Object {
    public:
        virtual void onCollision(std::shared_ptr<Tile> other) = 0;
        virtual void onCollision(std::shared_ptr<Enemy> other) = 0;
        virtual void onCollision(std::shared_ptr<Projectile> other) = 0;
        virtual void onCollision(std::shared_ptr<Player> other) = 0;
        
        bool getAttacking() const;
        virtual void attack() = 0;
        
        /*
        virtual void move();
        virtual void jump();
        virtual void dash();
        virtual void falling();
        virtual void takeDamage();
        */

    protected:
        Entity();

        bool isAttacking = false;
        unsigned max_health = 100;
        unsigned health = max_health;
};