#include "renderer.h"
#include "raylib.h"

Renderer::Renderer(Vec2* pos, Vec2* size) : pos(pos), size(size) { }

Renderer::~Renderer() {
    for (auto& sprite : sprites) {
        UnloadImage(sprite.second);
    }
}

void Renderer::saveSprite(std::string ID, std::string path) {
    Image image = LoadImage(path.c_str());
    ImageResize(&image, size->x, size->y);
    sprites.emplace(ID, image);
}

void Renderer::saveAnimation(std::string ID, std::string path, double fps) {
    animations.emplace(ID, Animation(path, fps, *size));
}

void Renderer::saveShader(std::string ID, CppShader& shader) {
    //shaders.emplace(ID, shader);
}

void Renderer::setState(std::string newTextureID, bool newFlipped) {
    if (newTextureID == "") newTextureID = currentTextureID;
    this->newTextureID = newTextureID;
    this->newFlipped = newFlipped;
}

void Renderer::setRotation(double rotation) {
    this->rotation = rotation;
}

void Renderer::addShader(std::string name) {
    shaders.emplace(name, CppShader());
}

Texture2D Renderer::updateCurrentTexture() {
    UnloadTexture(currentTexture);

    currentTextureID = newTextureID;
    Image& newImage = animations.find(currentTextureID) != animations.end() ? animations[currentTextureID].getCurrentFrame() : sprites[currentTextureID];

    if (newFlipped != flipped) {
        ImageFlipHorizontal(&newImage);
        flipped = newFlipped;
    }

    return LoadTextureFromImage(newImage);
}

void Renderer::update() {
    if (newTextureID != currentTextureID || newFlipped != flipped) {
        currentTexture = updateCurrentTexture();
    }

    Vec2 cornerPos = *pos - Vec2(currentTexture.width / 2, currentTexture.height / 2);
    float textureSize = size->x / currentTexture.width < size->y / currentTexture.height ? size->x / currentTexture.width : size->y / currentTexture.height;
    DrawTextureEx(currentTexture, cornerPos.toRaylib(), rotation, textureSize, WHITE);

    for (auto& shader : shaders) {
        //shader.second.apply();
    }
}