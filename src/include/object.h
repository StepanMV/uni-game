#pragma once
#include "renderer.h"
#include "physics.h"
#include "vec2.h"

class Object {
    public:
        Object() = default;

        virtual Object& spawn(Vec2 pos, Vec2 size);

        virtual void update() = 0;
        void render();

        Vec2 getPos() const;
        Vec2 getSize() const;


    protected:
        Vec2 pos;
        Vec2 size;
        Renderer renderer;
};