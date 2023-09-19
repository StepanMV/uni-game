#pragma once
#include "object.h"

class Entity: public Object {
    public:
        Entity(const unsigned _max_health);
        
        virtual void render() override;
        virtual void behaviour();
        virtual void move();
        virtual void jump();
        virtual void dash();
        virtual void falling();
        virtual void takeDamage();

        ~Entity();
    protected:
        const unsigned max_health;
        unsigned health = max_health;
        unsigned damage;//?
};