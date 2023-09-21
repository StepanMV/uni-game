#pragma once
#include <vector>
#include <string>
#include "raylib.h"
#include "vec2.h"

class Animation {
    public:
        Animation() = default;

        Animation(std::string filepath, double fps, Vec2 size);

        Image& getCurrentFrame();

        ~Animation();
    private:
        std::vector<Image> sprites;
        double lastChangeTimestamp;
        const double fps = 1;
        int currentFrame = 0;
};