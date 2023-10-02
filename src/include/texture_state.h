#pragma once

#include <memory>
#include <string>

#include "raylib.h"
#include "vec2.h"

enum class RendererType {
    TEXTURE,
    SPRITE_SHEET,
    ANIMATION
};

class TextureState {
    public:
        TextureState() = default;

        TextureState(std::string textureID, Vec2 textureSize);

        TextureState& spriteSheet(Vec2 size, Vec2 spritePos);

        TextureState& animation(Vec2 size, Vec2 startSpritePos, Vec2 endSpritePos, float fps);

        Rectangle getRect(bool flipped = false);

        std::string getTextureID() const;

        void setSpeed(float speed);

    private:
        std::string textureID;
        RendererType renderType = RendererType::TEXTURE;

        Vec2 size, frameSize;
        Vec2 startSpritePos, endSpritePos;

        float fps = 0;
        float speed = 1;
        float startTime = 0;
    };