#pragma once
#include "object.h"

class Projectile: public Object {
    public:
        Projectile(unsigned _damage, const bool _fromPlayer);

        void setSpeed(Vec2 speed);
        void setSpeed(Vec2 start, Vec2 target);

        virtual void update() override;
        virtual void render() override;
        void spawn(Vec2 pos, Vec2 size);
        void moveStraight();
        void moveHoming();
        void bounce();
        void redirect();
        void hit();
        void falling();

    private:
        unsigned damage;
        const bool fromPlayer;
        unsigned id;
};