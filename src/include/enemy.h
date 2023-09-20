#pragma once
#include "entity.h"

class Enemy: public Entity {
    public:
        Enemy();

        virtual void update() override;
        virtual void behaviour() override;
        virtual void move() override;
        virtual void jump() override;
        virtual void dash() override;
        virtual void falling() override;
        virtual void takeDamage() override;

        ~Enemy();
    private:
        //?
};