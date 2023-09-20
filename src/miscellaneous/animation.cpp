#include "animation.h"

#include <vector>
#include <string>
#include "raylib.h"

Animation::Animation(std::string filepath, double fps) : fps(fps) {
    this->lastChangeTimestamp = GetTime();
    auto list = LoadDirectoryFiles(filepath.c_str());
    for (int i = 0; i < list.count; i++) {
        sprites.push_back(LoadImage(list.paths[i]));
    }
}

Image Animation::getCurrentFrame() {
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