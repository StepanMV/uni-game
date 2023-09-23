#pragma once
#include "object.h"
#include "collider.h"
#include "vec2.h"
#include "physics.h"

class Entity: public Object {
    public:
        Entity() = default;
        Entity(Vec2 _pos, Vec2 _size);

        Entity& setMaxSpeeds(double move, double fall, double fly);

        Entity& setForces(double gravity, double friction);

        virtual void update() = 0;
        virtual void onCollision(Tile* other) = 0;
        virtual void onCollision(Entity* other) = 0;

        void move();
        
        //virtual void update() override;
        /*
        virtual void move();
        virtual void jump();
        virtual void dash();
        virtual void falling();
        virtual void takeDamage();
        */

    protected:
        const unsigned max_health = 100;
        unsigned health = max_health;
};