#pragma once

#include "entity.h"
#include "timer.h"
#include "projectile.h"
#include "weapon.h"

class Player: public Entity {
    public:
        virtual void update() override;
        virtual void render() override;

        virtual void onCollision(std::shared_ptr<Enemy> other) override;
        virtual void onCollision(std::shared_ptr<Player> other) override {};
        virtual void onCollision(std::shared_ptr<Projectile> other) override;

        virtual bool isCollideable() const override;

        static std::shared_ptr<Player> spawn(unsigned id, Vec2 pos, Vec2 size = Vec2(0, 0));

    private:
        virtual void readStats(std::string playerName);
        void moveEditor();
        int weaponId = 1;
        std::shared_ptr<Weapon> weapon;
        float max_friction;
        std::shared_ptr<Timer> dashTimer = Timer::getInstance(0.15);
        std::shared_ptr<Timer> runSoundTimer;
        std::shared_ptr<Timer> regenerateTimer = Timer::getInstance(1, true, [this](){health++;});
};