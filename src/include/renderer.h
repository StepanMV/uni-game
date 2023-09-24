#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

#include "cppshader.h"
#include "animation.h"
#include "texture_atlas.h"
#include "raylib.h"
#include "vec2.h"

enum class RendererType {
    TEXTURE,
    ANIMATION,
    TEXTURE_ATLAS
};



class Renderer {
public:
    Renderer() = default;
    Renderer(Vec2* pos, Vec2* size);

    void changeObject(Vec2* pos, Vec2* size);

    static void loadTextures(std::string folder);
    static void unloadTextures();

    void loadTexture(std::string ID, std::string filename, bool alsoFlipped = false);
    void loadTextureAtlas(std::string ID, std::string filename, Vec2 frameSize, bool alsoFlipped = false);
    void loadAnimation(std::string ID, std::string filename, double fps, Vec2 frameSize, bool alsoFlipped = false);

    void setMain(std::string ID, RendererType type, Vec2 pos = Vec2(0, 0));
    void addExtra(std::string ID, RendererType type, Vec2 pos = Vec2(0, 0));
    void removeExtra(std::string ID);
    void clearExtra();

    void setRotation(double rotation);
    void setScale(double scale);

    void update();

private:
    std::shared_ptr<Texture2D> getFromVRAM(std::string filename, bool flipped = false);
    void draw(std::string ID, RendererType type, Vec2 atlasPos);

    Vec2* pos;
    Vec2* size;
    double rotation = 0;
    double scale = 1;

    std::string mainTextureID;
    RendererType mainTextureType;
    Vec2 mainTextureAtlasPos;

    std::vector<std::string> extraTextureIDs;
    std::vector<RendererType> extraTextureTypes;
    std::vector<Vec2> extraTextureAtlasPoses;

    std::unordered_map<std::string, Animation> animations;
    std::unordered_map<std::string, TextureAtlas> textureAtlases; // Sprite sheets
    std::unordered_map<std::string, std::shared_ptr<Texture2D>> textures;

    static std::unordered_map<std::string, std::weak_ptr<Texture2D>> texturesVRAM;
    static std::unordered_map<std::string, Image> texturesRAM;

    //static std::unordered_map<std::string, Shader> shadersRAM;
    //static std::unordered_map<std::string, std::weak_ptr<CppShader>> shaders;
};