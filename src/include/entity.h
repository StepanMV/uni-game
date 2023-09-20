#pragma once
#include "object.h"
#include "collider.h"
#include "vec2.h"
#include "physics.h"

class Entity: public Object {
    public:
        Entity(const unsigned _max_health);
        
        virtual void update() override;
        virtual void move();
        virtual void jump();
        virtual void dash();
        virtual void falling();
        virtual void takeDamage();

        ~Entity();
    protected:
        Collider collider;
        Physics physics;
        const unsigned max_health;
        unsigned health = max_health;
        unsigned damage;//?
};