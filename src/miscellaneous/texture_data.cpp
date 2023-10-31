#include "texture_data.h"
#include <cmath>
#include <stdexcept>
#include <iostream>

std::string TextureData::getTextureID() const
{
    return textureID;
}

TextureType TextureData::getType() const {
    return type;
}

float TextureData::getRotation() const {
    return rotation;
}

bool TextureData::getFlipped() const {
    return flipped;
}

bool TextureData::isFree() const {
    return freeTransform;
}

Rectangle TextureData::getSourceRect() const {
    return Rectangle{0, 0, textureSize.x, textureSize.y};
}

Rectangle TextureData::getDestRect() const {
    return Rectangle{destOffset.x, destOffset.y, scale, scale};
}

void TextureData::setScale(float scale) {
    this->scale = scale;
}

void TextureData::setDestOffset(Vec2 destOffset) {
    this->destOffset = destOffset;
}

void TextureData::setRotation(float rotation) {
    this->rotation = rotation;
}

void TextureData::setFlipped(bool flipped) {
    this->flipped = flipped;
}

void CroppedTexture::setOffset(Vec2 offset)
{
    this->offset = offset;
}

void CroppedTexture::setFlipped(bool flipped) {
    this->flipped = flipped;
    offset.x = textureSize.x - offset.x;
    destOffset.x = -destOffset.x;
}

Rectangle CroppedTexture::getSourceRect() const {
    return Rectangle{offset.x, offset.y, textureSize.x, textureSize.y};
}

Rectangle SpriteSheet::getSourceRect() const {
    Vec2 textureSize = {this->textureSize.x / sheetDimensions.x, this->textureSize.y / sheetDimensions.y};
    return Rectangle{
        spritePos.x * textureSize.x + flipOffset,
        spritePos.y * textureSize.y,
        textureSize.x - spacing.x,
        textureSize.y - spacing.y
    };
}

Rectangle SpriteSheet::getDestRect() const {
    Vec2 textureSize = {this->textureSize.x / sheetDimensions.x, this->textureSize.y / sheetDimensions.y};
    return Rectangle{destOffset.x, destOffset.y, scale, scale};
}

void SpriteSheet::setSpritePos(const Vec2 &spritePos) {
    this->spritePos = spritePos;
}

void SpriteSheet::setFlipped(bool flipped) {
    if (this->flipped == flipped) return;
    this->flipped = flipped;
    spritePos.x = sheetDimensions.x - 1 - spritePos.x;
    destOffset.x = -destOffset.x;
    flipOffset = -flipOffset + spacing.x;
}

void Animation::setSpeed(float speedMultiplier) {
    this->speedMultiplier = speedMultiplier;
}

void Animation::setFlipped(bool flipped) {
    if (this-> flipped == flipped) return;
    this->flipped = flipped;
    startSpritePos.x = sheetDimensions.x - 1 - startSpritePos.x;
    endSpritePos.x = sheetDimensions.x - 1 - endSpritePos.x;
    destOffset.x = -destOffset.x;
    if (startSpritePos.x != endSpritePos.x) frame = -frame;
    flipOffset = -flipOffset + spacing.x;
}

void Animation::update() {
    int fps = this->fps * speedMultiplier;
    deltaTime += GetFrameTime();
    if (deltaTime >= 1.0 / fps) {
        deltaTime -= 1.0 / fps;
        frame += flipped && startSpritePos.x != endSpritePos.x ? -1 : 1;
        frame %= animSize;
    }
}

Rectangle Animation::getSourceRect() const {
    Vec2 textureSize = {this->textureSize.x / sheetDimensions.x, this->textureSize.y / sheetDimensions.y};
    if (startSpritePos.x == endSpritePos.x) {
        return Rectangle{
            startSpritePos.x * textureSize.x + flipOffset,
            (startSpritePos.y + frame) * textureSize.y,
            textureSize.x - spacing.x,
            textureSize.y - spacing.y
        };
    } else {
        return Rectangle{
            (startSpritePos.x + frame) * textureSize.x + flipOffset,
            startSpritePos.y * textureSize.y,
            textureSize.x - spacing.x,
            textureSize.y - spacing.y
        };
    }
}

Rectangle Animation::getDestRect() const {
    Vec2 textureSize = {this->textureSize.x / sheetDimensions.x, this->textureSize.y / sheetDimensions.y};
    return Rectangle{destOffset.x, destOffset.y, scale, scale};
}

TextureDataBuilder TextureDataBuilder::init(TextureType type, std::string textureID, Vec2 textureSize) {
    if (textureSize.x <= 0 || textureSize.y <= 0) throw std::runtime_error("Invalid texture size");
    TextureDataBuilder builder;
    switch (type) {
        case TextureType::TEXTURE:
            builder.textureData = std::shared_ptr<TextureData>(new TextureData());
            break;
        case TextureType::CROPPED_TEXTURE:
            builder.textureData = std::shared_ptr<TextureData>(new CroppedTexture());
            break;
        case TextureType::SPRITE_SHEET:
            builder.textureData = std::shared_ptr<TextureData>(new SpriteSheet());
            break;
        case TextureType::ANIMATION:
            builder.textureData = std::shared_ptr<TextureData>(new Animation());
            break;
    }
    const_cast<TextureType&>(builder.textureData->type) = type;
    const_cast<std::string&>(builder.textureData->textureID) = textureID;
    const_cast<Vec2&>(builder.textureData->textureSize) = textureSize;
    return builder;
}

TextureDataBuilder &TextureDataBuilder::setExtra(bool flipped, float rotation, float scale) {
    if (rotation < 0 || rotation >= 360) throw std::runtime_error("Invalid rotation");
    if (scale <= 0) throw std::runtime_error("Invalid scale");
    const_cast<bool&>(textureData->flipped) = flipped;
    const_cast<float&>(textureData->rotation) = rotation;
    const_cast<float&>(textureData->scale) = scale;
    return *this;
}

TextureDataBuilder &TextureDataBuilder::setDestOffset(Vec2 destOffset) {
    const_cast<Vec2&>(textureData->destOffset) = destOffset;
    return *this;
}

TextureDataBuilder &TextureDataBuilder::croppedTexture(Vec2 offset) {
    if (textureData->type != TextureType::CROPPED_TEXTURE) throw std::runtime_error("Invalid texture type");
    Vec2 textureSize = std::dynamic_pointer_cast<CroppedTexture>(textureData)->textureSize;
    if (offset.x < 0 || offset.y < 0 || offset.x >= textureSize.x || offset.y >= textureSize.y) throw std::runtime_error("Invalid offset");
    const_cast<Vec2&>(std::dynamic_pointer_cast<CroppedTexture>(textureData)->offset) = offset;
    return *this;
}

TextureDataBuilder &TextureDataBuilder::spriteSheet(Vec2 sheetDimensions, Vec2 spritePos, Vec2 spacing) {
    if (textureData->type != TextureType::SPRITE_SHEET) throw std::runtime_error("Invalid texture type");
    if (sheetDimensions.x <= 0 || sheetDimensions.y <= 0) throw std::runtime_error("Invalid sheet dimensions");
    if (spacing.x < 0 || spacing.y < 0) throw std::runtime_error("Invalid spacing");
    if (spritePos.x < 0 || spritePos.y < 0 || spritePos.x >= sheetDimensions.x || spritePos.y >= sheetDimensions.y) {
        throw std::runtime_error("Invalid sprite position");
    }
    const_cast<Vec2&>(std::dynamic_pointer_cast<SpriteSheet>(textureData)->sheetDimensions) = sheetDimensions;
    const_cast<Vec2&>(std::dynamic_pointer_cast<SpriteSheet>(textureData)->spacing) = spacing;
    const_cast<Vec2&>(std::dynamic_pointer_cast<SpriteSheet>(textureData)->spritePos) = spritePos;
    return *this;
}

TextureDataBuilder &TextureDataBuilder::animation(Vec2 sheetDimensions, Vec2 startSpritePos, Vec2 endSpritePos, float fps, Vec2 spacing) {
    if (textureData->type != TextureType::ANIMATION) throw std::runtime_error("Invalid texture type");
    if (sheetDimensions.x <= 0 || sheetDimensions.y <= 0) throw std::runtime_error("Invalid sheet dimensions");
    if (spacing.x < 0 || spacing.y < 0) throw std::runtime_error("Invalid spacing");
    if (startSpritePos.x < 0 || startSpritePos.y < 0 || startSpritePos.x >= sheetDimensions.x || startSpritePos.y >= sheetDimensions.y) {
        throw std::runtime_error("Invalid sprite position");
    }
    if (endSpritePos.x < 0 || endSpritePos.y < 0 || endSpritePos.x >= sheetDimensions.x || endSpritePos.y >= sheetDimensions.y) {
        throw std::runtime_error("Invalid sprite position");
    }
    if (startSpritePos.x == endSpritePos.x && startSpritePos.y == endSpritePos.y) {
        throw std::runtime_error("Invalid animation size");
    }
    if (startSpritePos.x != endSpritePos.x && startSpritePos.y != endSpritePos.y) {
        throw std::runtime_error("Invalid animation size");
    }
    if (fps <= 0) throw std::runtime_error("Invalid fps");
    const_cast<Vec2&>(std::dynamic_pointer_cast<Animation>(textureData)->sheetDimensions) = sheetDimensions;
    const_cast<Vec2&>(std::dynamic_pointer_cast<Animation>(textureData)->spacing) = spacing;
    const_cast<Vec2&>(std::dynamic_pointer_cast<Animation>(textureData)->startSpritePos) = startSpritePos;
    const_cast<Vec2&>(std::dynamic_pointer_cast<Animation>(textureData)->endSpritePos) = endSpritePos;
    const_cast<float&>(std::dynamic_pointer_cast<Animation>(textureData)->fps) = fps;
    const_cast<int&>(std::dynamic_pointer_cast<Animation>(textureData)->animSize) = std::abs(startSpritePos.x - endSpritePos.x) + std::abs(startSpritePos.y - endSpritePos.y) + 1;
    return *this;
}

TextureDataBuilder &TextureDataBuilder::keepProportions() {
    textureData->freeTransform = false;
    return *this;
}

std::shared_ptr<TextureData> TextureDataBuilder::build() {
    return textureData;
}
