#pragma once
#include "object.h"
#include "collider.h"
#include "vec2.h"

class Tile: public Object {
    public:
        Tile() = default;
        Tile(Vec2 _pos, Vec2 _size);

        virtual Tile& spawn() override;

        virtual void update() override;
        virtual void onCollision(Tile* other) override;
        virtual void onCollision(Entity* other) override;
};