#pragma once

#include "projectile.h"

enum class WeaponType {
    SWORD = 0,
    GUN
};

class Weapon: public Projectile {
    public:
        virtual void update() override;
        virtual void render() override;
        void setLeftSide(bool _leftSide);
        virtual void onCollision(std::shared_ptr<Enemy> other) override;
        static std::shared_ptr<Weapon> spawn(unsigned id, std::shared_ptr<MyTransform> refTransform, bool fromPlayer);
    private:
        Weapon() = default;
        std::shared_ptr<MyTransform> refTransform;
        double attackSpeed = 0;
        bool toLeft = false;
        WeaponType type;
};