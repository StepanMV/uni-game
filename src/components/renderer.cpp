#include "renderer.h"

#include <stdexcept>
#include <iostream>
#include <algorithm>

std::unordered_map<std::string, std::weak_ptr<Texture2D>> Renderer::texturesVRAM;
std::unordered_map<std::string, Image> Renderer::texturesRAM;

Renderer::Renderer(Vec2 *objectPos) : objectPos(objectPos) { }

void Renderer::changeObject(Vec2 *objectPos) {
    this->objectPos = objectPos;
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

size_t Renderer::getElementIndex(std::string element) {
    auto it = std::find(stateOrder.begin(), stateOrder.end(), element);
    if (it == stateOrder.end()) throw std::runtime_error("Invalid element");
    return it - stateOrder.begin();
}

void Renderer::loadTexture(std::string ID, std::string filename) {
    textures.emplace(ID, getFromVRAM(filename, false));
    textures.emplace(ID + " flipped", getFromVRAM(filename, true));
}

void Renderer::addToState(std::string stateID, std::string element, std::shared_ptr<TextureData> textureData) {
    objectStates[stateID].push_back(textureData);
    stateOrder.push_back(element);
}

Vec2 Renderer::getTextureSize(std::string ID) {
    return Vec2{textures[ID]->width, textures[ID]->height};
}

void Renderer::setState(std::string ID) {
    currentStateID = ID;
}

void Renderer::setFlipped(bool flipped, std::string element) {
    if (element.empty()) {
        for (auto& element : objectStates[currentStateID]) {
            element->setFlipped(flipped);
        }
    } else {
        objectStates[currentStateID][getElementIndex(element)]->setFlipped(flipped);
    }
}

void Renderer::setRotation(double rotation, std::string element)
{
    if (element.empty()) {
        for (auto& element : objectStates[currentStateID]) {
            element->setRotation(rotation);
        }
    } else {
        objectStates[currentStateID][getElementIndex(element)]->setRotation(rotation);
    }
}

void Renderer::setScale(double scale, std::string element) {
    if (element.empty()) {
        for (auto& element : objectStates[currentStateID]) {
            element->setScale(scale);
        }
    } else {
        objectStates[currentStateID][getElementIndex(element)]->setScale(scale);
    }
}

void Renderer::setDestOffset(Vec2 offset, std::string element) {
    if (element.empty()) {
        for (auto& element : objectStates[currentStateID]) {
            element->setDestOffset(offset);
        }
    } else {
        objectStates[currentStateID][getElementIndex(element)]->setDestOffset(offset);
    }
}

void Renderer::setOffset(Vec2 offset, std::string element) {
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

void Renderer::setSpritePos(Vec2 pos, std::string element) {
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

void Renderer::setAnimationSpeed(float speed, std::string element) {
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
void Renderer::render() {
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
        dest.x += objectPos->x;
        dest.y += objectPos->y;
        DrawTexturePro(*texture, source, dest, {dest.width / 2, dest.height / 2}, element->getRotation(), WHITE);
    }
}