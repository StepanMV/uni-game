#pragma once
#include "enemy.h"

class Slime: public Enemy {
    public:
        virtual void update() override;
        virtual void render() override;

        static std::shared_ptr<Slime> spawn(Vec2 pos, std::shared_ptr<Player> target);
    protected:
        void slimeBehavior();
        Slime() = default;
        double stayTime = 1.5;
        unsigned jumpCount = 1;
        std::shared_ptr<Timer> stayTimer = Timer::getInstance(stayTime + jumpTime);
};

class KingSlime: public Slime {
    public:
        virtual void update() override;
        static std::shared_ptr<KingSlime> spawn(Vec2 pos, std::shared_ptr<Player> target);
    private:
        KingSlime() = default;
        double currentScale = 1;
        const double tpTime = 0.5;
        std::shared_ptr<Timer> slimeSpanwTimer = Timer::getInstance(4);
        std::shared_ptr<Timer> betweenTpTimer = Timer::getInstance(10);
        std::shared_ptr<Timer> tpTimer = Timer::getInstance(tpTime);
        Vec2 tpPos = Vec2(0, 0);
};

class Eye: public Enemy {
    public:
        virtual void update() override;
        virtual void render() override {};
        static std::shared_ptr<Eye> spawn(Vec2 pos, std::shared_ptr<Player> target);
    private:
        Eye() = default;
};

class EyeOfCtulhu: public Enemy {
    public:
        virtual void update() override;
        virtual void render() override {};

        static std::shared_ptr<EyeOfCtulhu> spawn(Vec2 pos, std::shared_ptr<Player> target);
    private:
        EyeOfCtulhu() = default;

        void phase1();
        void phase2();
        void switchPhase();

        double maxFriction;
        unsigned phase = 1;
        const double chaseTime = 3;
        const double dashTime = 0.5;
        const double timeBetweenDashes = 0.75;
        const double switchTime = 5;
        double rotateSpeed = 0.5;
        int dashCount = 4;
        std::shared_ptr<Timer> eyeSpawnTimer = Timer::getInstance(2);
        std::shared_ptr<Timer> chaseTimer = Timer::getInstance(chaseTime);
        std::shared_ptr<Timer> switchTimer;
        std::shared_ptr<Timer> dashTimer = Timer::getInstance(dashTime);
        std::shared_ptr<Timer> betweenDashesTimer = Timer::getInstance(timeBetweenDashes);
};

class EowHead;

class EowSegment: public Enemy {
    public:
        virtual void update() override;
        virtual void render() override {};
    protected:
        std::shared_ptr<Timer> projTimer = Timer::getInstance(1);
        static std::shared_ptr<EowSegment> spawn(Vec2 pos, std::shared_ptr<Player> target);
        EowSegment() = default;
        std::shared_ptr<EowSegment> nextSegment;
        std::shared_ptr<EowSegment> prevSegment;
        friend class EowHead;
};

class EowHead: public EowSegment {
    public:
        virtual void update() override;
        virtual void render() override {};
        static std::shared_ptr<EowHead> spawn(Vec2 pos, std::shared_ptr<Player> target);
    private:
        EowHead() = default;
        const double rotateAngle = 4;
        std::shared_ptr<Timer> chaseTimer = Timer::getInstance(10);
        std::shared_ptr<Timer> restTimer = Timer::getInstance(10);
        std::shared_ptr<Timer> moveSoundTimer = Timer::getInstance(0.25);
        static std::shared_ptr<EowHead> spawnHead(Vec2 pos, std::shared_ptr<Player> target);
        friend class EowSegment;
};