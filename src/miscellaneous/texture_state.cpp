#include "texture_state.h"

#include "timer.h"
#include <stdexcept>

TextureState::TextureState(std::string textureID, Vec2 textureSize) {
    this->textureID = textureID;
    this->size = {1, 1};
    this->frameSize = textureSize;
    this->startSpritePos = this->endSpritePos = {0, 0};
}

TextureState &TextureState::spriteSheet(Vec2 size, Vec2 spritePos) {
    this->size = size;
    this->startSpritePos = spritePos;
    this->frameSize = {(float) (int) (frameSize.x / size.x), (float) (int) (frameSize.y / size.y)};
    this->renderType = RendererType::SPRITE_SHEET;
    return *this;
}

TextureState &TextureState::animation(Vec2 size, Vec2 startSpritePos, Vec2 endSpritePos, float fps) {
    if (startSpritePos.x != endSpritePos.x && startSpritePos.y != endSpritePos.y) {
        throw std::runtime_error("Animation can be only in one row or column");
    }
    this->size = size;
    this->startSpritePos = startSpritePos;
    this->endSpritePos = endSpritePos;
    this->frameSize = {(float) (int) (frameSize.x / size.x), (float) (int) (frameSize.y / size.y)};
    this->fps = fps;
    this->animSize = abs(endSpritePos.x - startSpritePos.x) + endSpritePos.y - startSpritePos.y + 1;
    this->renderType = RendererType::ANIMATION;
    return *this;
}

Rectangle TextureState::getRect(bool flipped) {
    float fps = this->fps * speedMultiplier;
    Vec2 startSpritePos = flipped ? Vec2 {size.x - this->startSpritePos.x - 1, this->startSpritePos.y} : this->startSpritePos;
    Vec2 endSpritePos = flipped ? Vec2 {size.x - this->endSpritePos.x - 1, this->endSpritePos.y} : this->endSpritePos;
    if (renderType == RendererType::ANIMATION) {
        double time = GetTime();
        if ((time - startTime) * fps >= animSize) startTime = time;
        frame += (time - startTime) > 1 / fps ? 1 : 0;
        frame %= animSize;

        if (startSpritePos.x == endSpritePos.x) {
            return Rectangle {
                startSpritePos.x * frameSize.x,
                (startSpritePos.y + frame) * frameSize.y,
                frameSize.x - 2,
                frameSize.y - 2
            };
        } else {
            return Rectangle {
                (startSpritePos.x + (flipped ? -frame : frame)) * frameSize.x,
                startSpritePos.y * frameSize.y,
                frameSize.x - 2,
                frameSize.y - 2
            };
        }
    } else {
        return Rectangle {
            startSpritePos.x * frameSize.x,
            startSpritePos.y * frameSize.y,
            frameSize.x - 2,
            frameSize.y - 2
        };
    }
}

std::string TextureState::getTextureID() const {
    return textureID;
}

void TextureState::setSpeed(float speedMultiplier) {
    this->speedMultiplier = speedMultiplier;
}
