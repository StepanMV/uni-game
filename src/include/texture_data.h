#pragma once

#include <memory>
#include <string>

#include "raylib.h"
#include "vec2.h"

enum class TextureType {
    TEXTURE,
    CROPPED_TEXTURE,
    SPRITE_SHEET,
    ANIMATION
};

class TextureData {
public:
    virtual ~TextureData() = default;    

    std::string getTextureID() const;
    TextureType getType() const;
    float getRotation() const;
    bool getFlipped() const;

    virtual Rectangle getSourceRect() const;
    virtual Rectangle getDestRect() const;

    void setScale(float scale);
    void setDestOffset(Vec2 destOffset);
    void setRotation(float rotation);
    virtual void setFlipped(bool flipped);

protected:
    friend class TextureDataBuilder;

    float rotation = 0;
    float scale = 1;
    bool flipped = false;
    Vec2 destOffset = {0, 0};

    const std::string textureID;
    const Vec2 textureSize;
    const TextureType type = TextureType::TEXTURE;
};

class CroppedTexture : public TextureData {
public:
    Rectangle getSourceRect() const override;

    void setOffset(Vec2 offset);

    void setFlipped(bool flipped) override;

private:
    friend class TextureDataBuilder;

    const TextureType type = TextureType::CROPPED_TEXTURE;
    Vec2 offset = {0, 0};
};

class SpriteSheet : public TextureData {
public:
    Rectangle getSourceRect() const override;
    Rectangle getDestRect() const override;

    void setSpritePos(const Vec2& spritePos);

    void setFlipped(bool flipped) override;

private:
    friend class TextureDataBuilder;

    const TextureType type = TextureType::SPRITE_SHEET;
    // textureSize is reduced to the size of a single sprite
    const Vec2 sheetDimensions; // number of sprites in the sheet
    const Vec2 spacing; // some textures have spacing between sprites
    Vec2 spritePos; // {0, 0} <= spritePos < sheetDimensions
    int flipOffset = 0;
};

class Animation : public TextureData {
public:
    Rectangle getSourceRect() const override;
    Rectangle getDestRect() const override;

    void setSpeed(float speedMultiplier);

    void setFlipped(bool flipped) override;

    void update();

private:
    friend class TextureDataBuilder;

    const TextureType type = TextureType::ANIMATION;
    const Vec2 sheetDimensions; // number of sprites in the sheet
    Vec2 startSpritePos, endSpritePos; // animation goes from startSpritePos to endSpritePos
    const Vec2 spacing;

    const int animSize = 0;
    const float fps = 0;
    float deltaTime = 0;
    int frame = 0;
    float speedMultiplier = 1;
    int flipOffset = 0;
};

class TextureDataBuilder {
public:
    static TextureDataBuilder init(TextureType type, std::string textureID, Vec2 textureSize);
    TextureDataBuilder& setExtra(bool flipped, float rotation, float scale);
    TextureDataBuilder& setDestOffset(Vec2 destOffset);
    TextureDataBuilder& croppedTexture(Vec2 offset);
    TextureDataBuilder& spriteSheet(Vec2 sheetDimensions, Vec2 spritePos, Vec2 spacing = {2, 2});
    TextureDataBuilder& animation(Vec2 sheetDimensions, Vec2 startSpritePos, Vec2 endSpritePos, float fps, Vec2 spacing = {2, 2});

    std::shared_ptr<TextureData> build();

private:
    std::shared_ptr<TextureData> textureData;
};