#pragma once
#include "renderer.h"
#include "physics.h"
#include "vec2.h"

class Object {
    public:
        virtual void update() = 0;

    protected:
        Vec2 pos;
        Vec2 size;
        Renderer renderer;
};