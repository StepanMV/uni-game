#pragma once

#include "vec2.h"
#include "raylib.h"

class Object;
class Tile;
class Entity;

class ICollidable {
    public:
        virtual void onCollision(Tile* other) = 0;
        virtual void onCollision(Entity* other) = 0;
        virtual bool checkCollision(const Object& other) const;
};