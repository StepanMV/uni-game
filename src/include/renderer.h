#pragma once

#include <unordered_map>
#include <map>
#include <string>
#include <memory>
#include <vector>

#include "texture_data.h"
#include "raylib.h"
#include "vec2.h"

enum TileType {
    DEFAULT_TILE,
    PLATFORM_TILE
};

class Renderer {
public:
    virtual ~Renderer() = default;
    Renderer() = default;
    Renderer(std::shared_ptr<MyTransform> transform);
    void changeObject(std::shared_ptr<MyTransform> transform);

    static void loadTextures(std::string folder);
    static void unloadTextures();

    virtual void render() = 0;

protected:
    static std::shared_ptr<Texture2D> getFromVRAM(std::string filename, bool flipped = false);

    std::shared_ptr<MyTransform> transform;

    static std::unordered_map<std::string, std::weak_ptr<Texture2D>> texturesVRAM;
    static std::unordered_map<std::string, Image> texturesRAM;
};

class TileRenderer : public Renderer {
public:
    TileRenderer() = default;
    TileRenderer(std::shared_ptr<MyTransform> transform);

    Vec2 loadTexture(std::string filename);
    void setSpritePos(unsigned short int state, TileType type = DEFAULT_TILE);

    void render() override;
private:
    static Vec2 spritePosDefault(unsigned short int state);
    static Vec2 spritePosPlatform(unsigned short int state);
    std::shared_ptr<Texture2D> texture;
    Vec2 spritePos;
};

class CoolRenderer : public Renderer {
public:
    CoolRenderer() = default;
    CoolRenderer(std::shared_ptr<MyTransform> transform);

    // Для инициализации
    Vec2 loadTexture(std::string ID, std::string filename);
    void addToState(std::string stateID, std::string element, std::shared_ptr<TextureData> textureData);
    Vec2 getTextureSize(std::string ID);

    void setState(std::string ID);


    void setFlipped(bool flipped, std::string element = "");
    void setColor(Color color, std::string element = "");
    void setAlpha(float alpha, std::string element = "");
    void setRotation(double rotation, std::string element = "");
    void setScale(double scale, std::string element = "");
    void setDestOffset(Vec2 offset, std::string element = "");
    void setOffset(Vec2 offset, std::string element = "");
    void setSpritePos(Vec2 pos, std::string element = "");
    void setAnimationSpeed(float speed, std::string element = "");

    void render() override;

private:
    size_t getElementIndex(std::string element);

    std::string currentStateID = "";

    std::map<std::string, std::vector<std::shared_ptr<TextureData>>> objectStates;
    std::vector<std::string> stateOrder;
    std::map<std::string, std::shared_ptr<Texture2D>> textures;
};