#include "animation.h"

#include <vector>
#include <string>
#include "raylib.h"
#include "vec2.h"

Animation::Animation(std::string filepath, double fps, Vec2 size) : fps(fps) {
    this->lastChangeTimestamp = GetTime();
    auto list = LoadDirectoryFiles(filepath.c_str());
    for (int i = 0; i < list.count; i++) {
        Image image = LoadImage(list.paths[i]);
        ImageResize(&image, size.x, size.y);
        sprites.push_back(image);
    }
}

Image& Animation::getCurrentFrame() {
    double now = GetTime();
    if (now - lastChangeTimestamp > 1.0 / fps) {
        lastChangeTimestamp = now;
        currentFrame++;
        if (currentFrame >= sprites.size()) {
            currentFrame = 0;
        }
    }
    return sprites[currentFrame];
}

Animation::~Animation() {
    for (auto sprite : sprites) {
        UnloadImage(sprite);
    }
}