#pragma once
#include "entity.h"

class Player: public Entity {
    public:
        Player();

        virtual void update() override;
        virtual void move() override;
        void moveRight();
        void moveLeft();
        virtual void jump() override;
        virtual void dash() override;
        virtual void falling() override;
        virtual void takeDamage() override;

        ~Player();
    private:
        //
};