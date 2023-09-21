#pragma once
#include <unordered_map>
#include <string>
#include "animation.h"
#include "cppshader.h"
#include "raylib.h"
#include "vec2.h"

class Renderer {
public:
    Renderer() = default;
    Renderer(Vec2* pos, Vec2* size);

    ~Renderer();

    void saveSprite(std::string ID, std::string path);

    void saveAnimation(std::string ID, std::string path, double fps);

    void saveShader(std::string ID, CppShader& shader);

    void setState(std::string ID, bool flip = false);

    void setRotation(double rotation);

    void addShader(std::string ID);

    void update();

private:
    Texture2D updateCurrentTexture();

    Vec2* pos;
    Vec2* size;
    double rotation = 0;
    bool flipped = false;
    std::unordered_map<std::string, Animation> animations;
    std::unordered_map<std::string, Image> sprites;
    std::unordered_map<std::string, CppShader> shaders;
    std::string currentTextureID;
    Texture2D currentTexture;

    std::string newTextureID;
    bool newFlipped = false;
};