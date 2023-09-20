#include "renderer.h"

Renderer::Renderer(Vec2* pos, int width, int height) {
    this->pos = pos;
    this->width = width;
    this->height = height;
}

void Renderer::saveSprite(std::string name, std::string path) {
    sprites.emplace(name, LoadImage(path.c_str()));
}

void Renderer::saveAnimation(std::string name, std::string path, double fps) {
    animations.emplace(name, Animation(path, fps));
}

void Renderer::saveShader(std::string name, CppShader& shader) {
    //shaders.emplace(name, shader);
}

void Renderer::setState(std::string name) {
    if (animations.find(name) != animations.end()) {
        currentAnimation = name;
        currentSprite = "";
    } else if (sprites.find(name) != sprites.end()) {
        currentSprite = name;
        currentAnimation = "";
    }
}

void Renderer::setRotation(double rotation) {
    this->rotation = rotation;
}

void Renderer::flip(bool flip) {
    if (flipped != flip) {
        ImageFlipHorizontal(&sprites[currentSprite]);
        flipped = flip;
    }
}

void Renderer::addShader(std::string name) {
    shaders.emplace(name, CppShader());
}

void Renderer::update() {

    Texture2D texture;
    if (currentAnimation != "") {
        texture = LoadTextureFromImage(animations[currentAnimation].getCurrentFrame());
    } else if (currentSprite != "") {
        texture = LoadTextureFromImage(sprites[currentSprite]);
    }
    Vec2 cornerPos = *pos - Vec2(texture.width / 2, texture.height / 2);
    DrawTextureEx(texture, cornerPos.toRaylib(), rotation, 1.0f, WHITE);

    for (auto& shader : shaders) {
        //shader.second.apply();
    }
}