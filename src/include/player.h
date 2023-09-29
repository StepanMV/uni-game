#pragma once

#include "entity.h"

class Player: public Entity {
    public:
        Player() = default;
        Player(Vec2 pos, Vec2 size);

        virtual Player& spawn(Vec2 pos, Vec2 size) override;

        virtual void update() override;
        virtual void onCollision(Tile& other) override;
        virtual void onCollision(Entity& other) override;
        /*
        virtual void move() override;
        void moveRight();
        void moveLeft();
        virtual void jump() override;
        virtual void dash() override;
        virtual void falling() override;
        virtual void takeDamage() override;
        */
    private:
        double startY = pos.y; // Не уверен что это здесь должно быть и должно ли вообще быть
};