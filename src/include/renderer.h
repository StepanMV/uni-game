#pragma once
#include <unordered_map>
#include <string>
#include "animation.h"
#include "cppshader.h"
#include "raylib.h"
#include "vec2.h"

class Renderer {
public:
    Renderer(Vec2* pos, int width, int height);

    void saveSprite(std::string name, std::string path);

    void saveAnimation(std::string name, std::string path, double fps);

    void saveShader(std::string name, CppShader& shader);

    void setState(std::string name);

    void setRotation(double rotation);

    void flip(bool flip = true);

    void addShader(std::string name);

    void update();

private:
    Vec2* pos;
    int width, height;
    double rotation = 0;
    bool flipped = false;
    std::unordered_map<std::string, Animation> animations;
    std::unordered_map<std::string, Image> sprites;
    std::unordered_map<std::string, CppShader> shaders;
    std::string currentAnimation;
    std::string currentSprite;
};