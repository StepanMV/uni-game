#pragma once
#include "enemy.h"

class Slime: public Enemy {
    public:
        virtual void update() override;
        virtual void render() override;
        void slimeBehavior();
    protected:
        double stayTime = 1.5;
        unsigned jumpCount = 1;
        std::shared_ptr<Timer> stayTimer = Timer::getInstance(stayTime + jumpTime);
};

class KingSlime: public Slime {
    public:
        virtual void update() override;
    private:
        double currentScale = 1;
        const double tpTime = 0.5;
        std::shared_ptr<Timer> slimeSpanwTimer = Timer::getInstance(4);
        std::shared_ptr<Timer> betweenTpTimer = Timer::getInstance(10);
        std::shared_ptr<Timer> tpTimer = Timer::getInstance(tpTime);
        Vec2 tpPos = Vec2(0, 0);
};