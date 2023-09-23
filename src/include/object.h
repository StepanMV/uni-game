#pragma once
#include "renderer.h"
#include "physics.h"
#include "collider.h"

class Object: public ICollidable {
    public:
        Object() = default;
        Object(Vec2 _pos, Vec2 _size);

        virtual Object& spawn();

        virtual void update() = 0;
        void render();

        Vec2 getPos() const;
        Vec2 getSize() const;


    protected:
        Renderer renderer;
        Physics physics;
};