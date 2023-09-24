#pragma once
#include "object.h"
#include "vec2.h"

class Tile: public Object {
    public:
        Tile() = default;

        Tile(Vec2 pos, Vec2 size);

        virtual void update() override;
};