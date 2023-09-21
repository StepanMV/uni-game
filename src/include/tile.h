#pragma once
#include "object.h"
#include "collider.h"
#include "vec2.h"

class Tile: public Object, public ICollidable {
    public:
        Tile() = default;

        virtual Tile& spawn(Vec2 pos, Vec2 size) override;

        virtual void update() override;
        virtual void onCollision(Tile* other) override;
        virtual void onCollision(Entity* other) override;
        virtual bool checkCollision(const Object& other) const override;
};