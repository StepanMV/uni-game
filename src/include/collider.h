#pragma once

#include "vec2.h"
#include "raylib.h"

class Object;
class Tile;
class Entity;

class ICollidable {
    public:
        ICollidable(Vec2 _pos, Vec2 _size);

        virtual void onCollision(Tile* other) = 0;
        virtual void onCollision(Entity* other) = 0;
        bool checkCollision(const Object& other) const;
        Rectangle recCollision(const Object& other) const;
        Rectangle recCollision(const Vec2& otherPos, const Vec2& otherSize) const;
    protected:
        Vec2 pos;
        Vec2 size;
};