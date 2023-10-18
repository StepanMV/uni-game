#include "renderer.h"

#include <stdexcept>
#include <iostream>
#include <algorithm>

std::unordered_map<std::string, std::weak_ptr<Texture2D>> Renderer::texturesVRAM;
std::unordered_map<std::string, Image> Renderer::texturesRAM;

Renderer::Renderer(std::shared_ptr<MyTransform> _transform) : transform(_transform) { }

void Renderer::changeObject(std::shared_ptr<MyTransform> transform) {
    this->transform = transform;
}

void Renderer::loadTextures(std::string folder)
{
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

    for (auto it = texturesVRAM.begin(); it != texturesVRAM.end();) {
        if (it->second.expired()) it = texturesVRAM.erase(it);
        else ++it;
    }
    
    if (texturesVRAM.find(textureName) == texturesVRAM.end()) {
        if (flipped) ImageFlipHorizontal(&texturesRAM[filename]);
        auto texturePtr = std::shared_ptr<Texture2D>(new Texture2D(LoadTextureFromImage(texturesRAM[filename])), [](Texture2D* texture) { UnloadTexture(*texture); });
        if (flipped) ImageFlipHorizontal(&texturesRAM[filename]);
        texturesVRAM.emplace(textureName, std::weak_ptr<Texture2D>(texturePtr));
        return texturePtr;
    } else {
        return texturesVRAM[textureName].lock();
    }
}

size_t CoolRenderer::getElementIndex(std::string element) {
    auto it = std::find(stateOrder.begin(), stateOrder.end(), element);
    if (it == stateOrder.end()) throw std::runtime_error("Invalid element");
    return it - stateOrder.begin();
}

CoolRenderer::CoolRenderer(std::shared_ptr<MyTransform> transform) : Renderer(transform) { }

Vec2 CoolRenderer::loadTexture(std::string ID, std::string filename)
{
    textures.emplace(ID, getFromVRAM(filename, false));
    textures.emplace(ID + " flipped", getFromVRAM(filename, true));
    return {(float) textures[ID]->width, (float) textures[ID]->height};
}

void CoolRenderer::addToState(std::string stateID, std::string element, std::shared_ptr<TextureData> textureData) {
    objectStates[stateID].push_back(textureData);
    stateOrder.push_back(element); 
}

Vec2 CoolRenderer::getTextureSize(std::string ID) {
    return Vec2{(float) textures[ID]->width, (float) textures[ID]->height};
}

void CoolRenderer::setState(std::string ID) {
    currentStateID = ID;
}

void CoolRenderer::setFlipped(bool flipped, std::string element) {
    if (element.empty()) {
        for (auto& element : objectStates[currentStateID]) {
            element->setFlipped(flipped);
        }
    } else {
        objectStates[currentStateID][getElementIndex(element)]->setFlipped(flipped);
    }
}

void CoolRenderer::setRotation(double rotation, std::string element)
{
    if (element.empty()) {
        for (auto& element : objectStates[currentStateID]) {
            element->setRotation(rotation);
        }
    } else {
        objectStates[currentStateID][getElementIndex(element)]->setRotation(rotation);
    }
}

void CoolRenderer::setScale(double scale, std::string element) {
    if (element.empty()) {
        for (auto& element : objectStates[currentStateID]) {
            element->setScale(scale);
        }
    } else {
        objectStates[currentStateID][getElementIndex(element)]->setScale(scale);
    }
}

void CoolRenderer::setDestOffset(Vec2 offset, std::string element) {
    if (element.empty()) {
        for (auto& element : objectStates[currentStateID]) {
            element->setDestOffset(offset);
        }
    } else {
        objectStates[currentStateID][getElementIndex(element)]->setDestOffset(offset);
    }
}

void CoolRenderer::setOffset(Vec2 offset, std::string element) {
    if (element.empty()) {
        for (auto& element : objectStates[currentStateID]) {
            if (element->getType() == TextureType::CROPPED_TEXTURE) {
                std::dynamic_pointer_cast<CroppedTexture>(element)->setOffset(offset);
            }
        }
    } else {
        size_t index = getElementIndex(element);
        if (objectStates[currentStateID][index]->getType() != TextureType::CROPPED_TEXTURE) throw std::runtime_error("Changing invalid texture type");
        std::dynamic_pointer_cast<CroppedTexture>(objectStates[currentStateID][index])->setOffset(offset);
    }
}

void CoolRenderer::setSpritePos(Vec2 pos, std::string element) {
    if (element.empty()) {
        for (auto& element : objectStates[currentStateID]) {
            if (element->getType() == TextureType::SPRITE_SHEET) {
                std::dynamic_pointer_cast<SpriteSheet>(element)->setSpritePos(pos);
            }
        }
    } else {
        size_t index = getElementIndex(element);
        if (objectStates[currentStateID][index]->getType() != TextureType::SPRITE_SHEET) throw std::runtime_error("Changing invalid texture type");
        std::dynamic_pointer_cast<CroppedTexture>(objectStates[currentStateID][index])->setOffset(pos);
    }
}

void CoolRenderer::setAnimationSpeed(float speed, std::string element) {
    if (element.empty()) {
        for (auto& element : objectStates[currentStateID]) {
            if (element->getType() == TextureType::ANIMATION) {
                std::dynamic_pointer_cast<Animation>(element)->setSpeed(speed);
            }
        }
    } else {
        size_t index = getElementIndex(element);
        if (objectStates[currentStateID][index]->getType() != TextureType::ANIMATION) throw std::runtime_error("Changing invalid texture type");
        std::dynamic_pointer_cast<Animation>(objectStates[currentStateID][index])->setSpeed(speed);
    }
}
void CoolRenderer::render() {
    if (currentStateID.empty()) return;
    for (auto& element : objectStates[currentStateID]) {
        if (element->getType() == TextureType::ANIMATION) {
            std::dynamic_pointer_cast<Animation>(element)->update();
        }
    }

    for (auto& element : objectStates[currentStateID]) {
        std::string textureID = element->getTextureID();
        textureID = element->getFlipped() ? textureID + " flipped" : textureID;
        auto texture = textures[textureID];
        Rectangle source = element->getSourceRect();
        Rectangle dest = element->getDestRect();
        dest.x += transform->pos.x;
        dest.y += transform->pos.y;
        DrawTexturePro(*texture, source, dest, {dest.width / 2, dest.height / 2}, element->getRotation(), WHITE);
    }
}

TileRenderer::TileRenderer(std::shared_ptr<MyTransform> transform) : Renderer(transform) { }

Vec2 TileRenderer::loadTexture(std::string filename) {
    texture = getFromVRAM(filename);
    return {(float) texture->width, (float) texture->height};
}

void TileRenderer::setSpritePos(unsigned short int state) {
    spritePos = getSpritePos(state);
}

void TileRenderer::render() {
    Rectangle source = {spritePos.x * 18, spritePos.y * 18, 16, 16};
    Rectangle dest = {transform->pos.x, transform->pos.y, 16, 16};
    DrawTexturePro(*texture, source, dest, {dest.width / 2, dest.height / 2}, 0, WHITE);
}

Vec2 TileRenderer::getSpritePos(unsigned short int state) {
    switch(state) {
        case 0b0000: return {(float) GetRandomValue(9, 11), 3};
        case 0b0001: return {9, (float) GetRandomValue(0, 2)};
        case 0b0010: return {12, (float) GetRandomValue(0, 2)};
        case 0b0011: return {(float) GetRandomValue(6, 8), 4};
        case 0b0100: return {(float) GetRandomValue(6, 8), 0};
        case 0b0101: return {0, 3};
        case 0b0110: return {1, 3};
        case 0b0111: return {(float) GetRandomValue(1, 3), 0};
        case 0b1000: return {(float) GetRandomValue(6, 8), 3};
        case 0b1001: return {0, 4};
        case 0b1010: return {1, 4};
        case 0b1011: return {(float) GetRandomValue(1, 3), 2};
        case 0b1100: return {5, (float) GetRandomValue(0, 2)};
        case 0b1101: return {0, (float) GetRandomValue(0, 2)};
        case 0b1110: return {4, (float) GetRandomValue(0, 2)};
        case 0b1111: return {(float) GetRandomValue(1, 3), 1};
        default: return {(float) GetRandomValue(9, 11), 3};
    }
}
