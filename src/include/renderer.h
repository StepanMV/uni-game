#pragma once
#include <map>
#include <string>
#include "animation.h"
#include "myshader.h"
#include "raylib.h"

class Renderer {
public:
    Renderer(int* x, int* y, int width, int height);

    void saveTexture(std::string name, std::string path);

    void saveAnimation(std::string name, std::string path, int speed);

    void saveShader(std::string name, MyShader& shader);

    void setTexture(std::string name);

    void setAnimation(std::string name);

    void setRotation(double rotation);

    void addShader(std::string name);

    void update();

private:
    int *x, *y;
    int width, height;
    std::map<std::string, Animation> animations;
    std::map<std::string, Texture2D> textures;
    std::map<std::string, MyShader> shaders;
    std::string currentAnimation;
    std::string currentTexture;
};