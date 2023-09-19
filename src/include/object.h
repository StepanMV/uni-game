#pragma once
#include "renderer.h"
#include "physics.h"
#include "vec2.h"

class Object {
    public:
        virtual void render() = 0;
    protected:
        int x, y;//vec2?
        Renderer renderer;
        Physics physics;
};