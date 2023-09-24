#include "renderer.h"

#include <iostream>

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
        std::cout << "Loaded texture " << filename << "Usages: " << texturePtr.use_count() << std::endl;
        return texturePtr;
    } else {
        std::cout << "Using loaded texture " << filename << " with " << texturesVRAM[textureName].use_count() << " usages, expired" << texturesVRAM[textureName].expired() << std::endl;
        return texturesVRAM[textureName].lock();
    }
}

void Renderer::loadTexture(std::string ID, std::string filename, bool flipped) {
    std::string textureName = filename + (flipped ? " flipped" : "");
    textures.emplace(ID, getFromVRAM(filename, flipped));
}

void Renderer::loadTextureAtlas(std::string ID, std::string filename, Vec2 frameSize, bool flipped) {
    std::string textureName = filename + (flipped ? " flipped" : "");
    textureAtlases.emplace(ID, TextureAtlas(getFromVRAM(filename, flipped), frameSize));
}

void Renderer::loadAnimation(std::string ID, std::string filename, double fps, Vec2 frameSize, bool flipped) {
    std::string textureName = filename + (flipped ? " flipped" : "");
    animations.emplace(ID, Animation(getFromVRAM(filename, flipped), fps, frameSize));
}

void Renderer::setMain(std::string ID, RendererType type, Vec2 pos) {
    mainTextureID = ID;
    mainTextureType = type;
    mainTextureAtlasPos = pos;
}

void Renderer::addExtra(std::string ID, RendererType type, Vec2 pos) {
    extraTextureIDs.push_back(ID);
    extraTextureTypes.push_back(type);
    extraTextureAtlasPoses.push_back(pos);
}

void Renderer::removeExtra(std::string ID) {
    for (int i = 0; i < extraTextureIDs.size(); i++) {
        if (extraTextureIDs[i] == ID) {
            extraTextureIDs.erase(extraTextureIDs.begin() + i);
            extraTextureTypes.erase(extraTextureTypes.begin() + i);
            extraTextureAtlasPoses.erase(extraTextureAtlasPoses.begin() + i);
            return;
        }
    }
}

void Renderer::clearExtra() {
    extraTextureIDs.clear();
    extraTextureTypes.clear();
    extraTextureAtlasPoses.clear();
}

void Renderer::setRotation(double rotation) {
    this->rotation = rotation;
}

void Renderer::setScale(double scale) {
    this->scale = scale;
}

void Renderer::draw(std::string ID, RendererType type, Vec2 atlasPos) {
    Texture2D texture;
    Rectangle source;
    Rectangle dest;

    switch (type) {
        case RendererType::TEXTURE:
            texture = *textures[ID];
            source = Rectangle {0, 0, (float) texture.width, (float) texture.height};
            break;
        case RendererType::TEXTURE_ATLAS:
            texture = *textureAtlases[ID].texture;
            source = textureAtlases[ID].getRect(atlasPos);
            break;
        case RendererType::ANIMATION:
            texture = *animations[ID].texture;
            source = animations[ID].getCurrentRect();
            break;
    }
    
    dest = Rectangle {(float) pos->x, (float) pos->y, (float) size->x, (float) size->y};
    Vector2 origin = {dest.width / 2, dest.height / 2};


    DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
}

void Renderer::update() {
    draw(mainTextureID, mainTextureType, mainTextureAtlasPos);
    for (int i = 0; i < extraTextureIDs.size(); i++) {
        draw(extraTextureIDs[i], extraTextureTypes[i], extraTextureAtlasPoses[i]);
    }
}