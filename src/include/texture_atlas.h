#pragma once

#include <memory>

#include "raylib.h"
#include "vec2.h"

class TextureAtlas {
    public:
        TextureAtlas() = default;

        TextureAtlas(std::shared_ptr<Texture2D> texture, Vec2 frameSize);

        Rectangle getRect(Vec2 pos);
        
        std::shared_ptr<Texture2D> texture;

    private:
        Vec2 frameSize;
        Vec2 size;
    };