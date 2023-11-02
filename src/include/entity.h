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
        virtual void destroy() override;

    protected:
        Entity();
        void move(Vec2 direction);
        void jump();
        void fly();
        void plane();
        void dash(Vec2 direction, double dashSpeed);
        void falling();
        void takeDamage(unsigned damage);
        void takeKnockback(float projPosX);
        virtual void attack() = 0;
        void onBoard();
        bool facingLeft = false;
        bool canClimb = false;
        std::shared_ptr<Timer> platformTimer = Timer::getInstance(0.15);
        std::string damageSound = "";
        bool isAttacking = false;
        bool skipPlatform = false;
        bool isFlying = false, onGround = false;
        double maxFlightTime = 2;
        double currentFlightTime = maxFlightTime;
        bool knockbackResist = true;
        double jumpTime = 0.2;
        double damageTime = 0;
        std::shared_ptr<Timer> flyTimer = Timer::getInstance(currentFlightTime);
        std::shared_ptr<Timer> jumpTimer = Timer::getInstance(jumpTime);
        std::shared_ptr<Timer> damageTimer = Timer::getInstance(damageTime);
        unsigned max_health = 400;
        float health = max_health;
        friend class Level;
};