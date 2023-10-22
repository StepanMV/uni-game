#pragma once

#include "projectile.h"

enum WeaponType {
    SWORD = 0,
    GUN
};

class Weapon: public Projectile {
    public:
        virtual void update() override;
        virtual void render() override;
        void setLeftSide(bool _leftSide);
    private:
        Weapon() = default;
        std::shared_ptr<MyTransform> refTransform;
        double attackSpeed = 0;
        bool leftSide = false;
        WeaponType type;
        friend class WeaponBuilder;
};

class WeaponBuilder {
    public:
        static WeaponBuilder spawn(std::shared_ptr<MyTransform> refTransform, Vec2 size, unsigned id);
        WeaponBuilder& extra(double _attackTime, unsigned _damage, WeaponType type, bool _fromPlayer);
        std::shared_ptr<Weapon> build();
    private:
        std::shared_ptr<Weapon> weapon;
};