#include "renderer.h"

std::unordered_map<std::string, std::weak_ptr<Texture2D>> Renderer::texturesVRAM;
std::unordered_map<std::string, Image> Renderer::texturesRAM;

Renderer::Renderer(Vec2* pos, Vec2* size) : pos(pos), size(size) { }

void Renderer::changeObject(Vec2* pos, Vec2* size) {
    this->pos = pos;
    this->size = size;
}

void Renderer::loadTextures(std::string folder) {
    auto list = LoadDirectoryFiles(folder.c_str());
    for (int i = 0; i < list.count; i++) texturesRAM.emplace(list.paths[i], LoadImage(list.paths[i]));
}

void Renderer::unloadTextures() {
    for (auto& texture : texturesRAM) {
        UnloadImage(texture.second);
    }
}

std::shared_ptr<Texture2D> Renderer::getFromVRAM(std::string filename, bool flipped) {
    std::string textureName = filename + (flipped ? " flipped" : "");
    if (texturesVRAM.find(textureName) == texturesVRAM.end() || texturesVRAM[textureName].expired()) {
        if (flipped) ImageFlipHorizontal(&texturesRAM[filename]);
        auto texturePtr = std::shared_ptr<Texture2D>(new Texture2D(LoadTextureFromImage(texturesRAM[filename])), [](Texture2D* texture) { UnloadTexture(*texture); });
        if (flipped) ImageFlipHorizontal(&texturesRAM[filename]);
        texturesVRAM.emplace(textureName, std::weak_ptr<Texture2D>(texturePtr));
        return texturePtr;
    } else {
        return texturesVRAM[textureName].lock();
    }
}

void Renderer::loadTexture(std::string ID, std::string filename) {
    textures.emplace(ID, getFromVRAM(filename, false));
    flippedTextures.emplace(ID, getFromVRAM(filename, true));
}

TextureState &Renderer::addToState(std::string stateID, std::string textureID) {
    auto texture = textures[textureID];
    objectStates[stateID].push_back(TextureState(textureID, {(float) texture->width, (float) texture->height}));
    return objectStates[stateID].back();
}

void Renderer::setState(std::string ID, bool flipped, float speed) {
    currentStateID = ID;
    currentStateFlipped = flipped;
    for (auto& element : objectStates[currentStateID]) element.setSpeed(speed);
}

void Renderer::setRotation(double rotation) {
    this->rotation = rotation;
}

void Renderer::setScale(double scale) {
    this->scale = scale;
}

void Renderer::update() {
    if (currentStateID.empty()) return;
    for (auto& element : objectStates[currentStateID]) {
        std::string textureID = element.getTextureID();
        auto texture = currentStateFlipped ? flippedTextures[textureID] : textures[textureID];
        Rectangle source = element.getRect(currentStateFlipped);
        Rectangle dest = Rectangle {(float) pos->x, (float) pos->y, (float) size->x, (float) size->y};
        Vector2 origin = {dest.width / 2, dest.height / 2};
        DrawTexturePro(*texture, source, dest, origin, rotation, WHITE);
    }
}