#pragma once

#include "projectile.h"

class Weapon: public Projectile {
    public:
        virtual void update() override;
        virtual void render() override;
        void setAngle(float angle);
        float getAngle();
        void setLeftSide(bool _leftSide);
    private:
        Weapon() = default;
        std::shared_ptr<MyTransform> refTransform;
        double attackSpeed = 0;
        bool leftSide = false;
        friend class WeaponBuilder;
};

class WeaponBuilder {
    public:
        static WeaponBuilder spawn(std::shared_ptr<MyTransform> refTransform, unsigned id, Vec2 size);
        WeaponBuilder& extra(double _attackTime, unsigned _damage, bool _fromPlayer);
        std::shared_ptr<Weapon> build();
    private:
        std::shared_ptr<Weapon> weapon;
};