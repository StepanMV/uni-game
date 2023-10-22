#pragma once

#include "object.h"
#include "timer.h"

class Tile;
class Projectile;

class Entity: public Object {
    public:
        virtual void onCollision(std::shared_ptr<Tile> other) override;
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
       void takeDamage(unsigned damage);

    protected:
        Entity();
        bool facingLeft = false;
        std::shared_ptr<Timer> platformTimer = Timer::getInstance(0.15);
        bool isAttacking = false;
        bool skipPlatform = false;
        bool isFlying = false, onGround = false;
        double maxFlightTime = 2;
        double currentFlightTime = maxFlightTime;
        double jumpTime = 0.3;
        std::shared_ptr<Timer> flyTimer = Timer::getInstance(currentFlightTime);
        std::shared_ptr<Timer> jumpTimer = Timer::getInstance(jumpTime);
        unsigned max_health = 100;
        unsigned health = max_health;
};