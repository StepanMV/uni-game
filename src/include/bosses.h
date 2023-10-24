#pragma once
#include "enemy.h"

class KingSlime: public Enemy {
    public:
        virtual void update() override;
        virtual void render() override;
    private:
        const double stayTime = 1.5;
        double scale = 1;
        const double tpTime = 0.5;
        unsigned jumpCount = 1;
        std::shared_ptr<Timer> stayTimer = Timer::getInstance(stayTime + jumpTime);
        std::shared_ptr<Timer> betweenTpTimer = Timer::getInstance(10);
        std::shared_ptr<Timer> tpTimer = Timer::getInstance(tpTime);
        Vec2 tpPos = Vec2(0, 0);
};