#pragma once

#include "object.h"
#include "timer.h"

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
        
       void move(Vec2 direction);
       void jump();
       void fly();
       void plane();
       void dash(Vec2 direction);
       void falling();
       void takeDamage() {};

    protected:
        Entity();
        bool isAttacking = false;
        bool isFlying = false;
        double maxFlightTime = 2;
        double currentFlightTime = maxFlightTime;
        std::shared_ptr<Timer> flyTimer = Timer::getInstance(currentFlightTime);
        unsigned max_health = 100;
        unsigned health = max_health;
};