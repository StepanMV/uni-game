#pragma once
#include "object.h"
#include "vec2.h"

class Tile: public Object {
    public:
        Tile() = default;
        Tile(Vec2 pos, Vec2 size);
        virtual Tile& spawn(Vec2 pos, Vec2 size) override;

        virtual void update() override;
};