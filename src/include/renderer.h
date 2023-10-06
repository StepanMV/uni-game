#pragma once

#include <unordered_map>
#include <map>
#include <string>
#include <memory>
#include <vector>

#include "cppshader.h"
#include "texture_data.h"
#include "raylib.h"
#include "vec2.h"

class Renderer {
public:
    Renderer(Vec2* objectPos);
    void changeObject(Vec2* objectPos);

    static void loadTextures(std::string folder);
    static void unloadTextures();

    // Для инициализации
    void loadTexture(std::string ID, std::string filename);
    void addToState(std::string stateID, std::string element, std::shared_ptr<TextureData> textureData);
    Vec2 getTextureSize(std::string ID);

    void setState(std::string ID);

    void setFlipped(bool flipped, std::string element = "");
    void setRotation(double rotation, std::string element = "");
    void setScale(double scale, std::string element = "");
    void setDestOffset(Vec2 offset, std::string element = "");
    void setOffset(Vec2 offset, std::string element = "");
    void setSpritePos(Vec2 pos, std::string element = "");
    void setAnimationSpeed(float speed, std::string element = "");

    void render();

protected:
    std::shared_ptr<Texture2D> getFromVRAM(std::string filename, bool flipped = false);
    size_t getElementIndex(std::string element);

    Vec2* objectPos;

    std::string currentStateID = "";

    std::map<std::string, std::vector<std::shared_ptr<TextureData>>> objectStates;
    std::vector<std::string> stateOrder;
    std::map<std::string, std::shared_ptr<Texture2D>> textures;

    static std::unordered_map<std::string, std::weak_ptr<Texture2D>> texturesVRAM;
    static std::unordered_map<std::string, Image> texturesRAM;

    //static std::unordered_map<std::string, Shader> shadersRAM;
    //static std::unordered_map<std::string, std::weak_ptr<CppShader>> shaders;
};

class TileRenderer : public Renderer {
private:
    std::shared_ptr<Texture2D> texture;
    Vec2 sheetPos;
};

class CoolRenderer : public Renderer {
private:
};


/* 
tile renderer..?
data: ptr to texture, sheet pos
*/