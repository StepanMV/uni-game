#pragma once
#include "enemy.h"

class Boss: public Enemy {
    public:
        Boss();

        virtual void render() override;
        

        ~Boss();
    private:
        //?
};