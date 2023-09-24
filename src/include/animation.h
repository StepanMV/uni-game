#pragma once

#include <memory>

#include "raylib.h"
#include "vec2.h"

class Animation {
    public:
        Animation() = default;

        Animation(std::shared_ptr<Texture2D> texture, double fps, Vec2 frameSize);

        Rectangle getCurrentRect();

        void restart();
        
        std::shared_ptr<Texture2D> texture;

    private:
        Vec2 frameSize;
        double fps = 1;
        double startTime = 0;
        Vec2 size;
    };