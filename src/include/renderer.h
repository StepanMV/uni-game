#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

#include "cppshader.h"
#include "texture_state.h"
#include "raylib.h"
#include "vec2.h"

class Renderer {
public:
    Renderer() = default;

    static void loadTextures(std::string folder);
    static void unloadTextures();

    // Для инициализации
    void loadTexture(std::string ID, std::string filename);
    TextureState& addToState(std::string stateID, std::string textureID);

    void setState(std::string ID, bool flipped = false, float speed = 1);
    void setRotation(double rotation);
    void setScale(double scale);

    void update(const Vec2& pos, const Vec2& size);
    void render();

private:
    std::shared_ptr<Texture2D> getFromVRAM(std::string filename, bool flipped = false);

    Vec2 pos, size;
    double rotation = 0;
    double scale = 1;

    std::string currentStateID = "";
    bool currentStateFlipped = false;
    float animationSpeed = 0;

    std::unordered_map<std::string, std::vector<TextureState>> objectStates;
    std::unordered_map<std::string, std::shared_ptr<Texture2D>> textures;
    std::unordered_map<std::string, std::shared_ptr<Texture2D>> flippedTextures;

    static std::unordered_map<std::string, std::weak_ptr<Texture2D>> texturesVRAM;
    static std::unordered_map<std::string, Image> texturesRAM;

    //static std::unordered_map<std::string, Shader> shadersRAM;
    //static std::unordered_map<std::string, std::weak_ptr<CppShader>> shaders;
};