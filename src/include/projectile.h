#pragma once
#include "object.h"

class Projectile: public Object {
    public:
        Projectile(unsigned _damage);

        virtual void update() override;
        void moveStraight();
        void moveHoming();
        void bounce();
        void redirect();
        void hit();
        void falling();

        ~Projectile();
    private:
        unsigned damage;
};