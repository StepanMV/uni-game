#include "animation.h"

#include "raylib.h"
#include "vec2.h"

Animation::Animation(std::shared_ptr<Texture2D> texture, double fps, Vec2 frameSize)
    :
    texture(texture),
    fps(fps),
    frameSize(frameSize),
    startTime(GetTime()),
    size(Vec2((int) (texture->width / frameSize.x), (int) (texture->height / frameSize.y)))
    { }

void Animation::restart() {
    startTime = GetTime();
}

Rectangle Animation::getCurrentRect() {
    double time = GetTime();
    if ((time - startTime) * fps >= size.x * size.y) startTime = time;
    int frame = (int) ((time - startTime) * fps) % (int) (size.x * size.y);
    return Rectangle {
        (float) ((frame % (int) size.x) * (float) frameSize.x),
        (float) ((frame / (int) size.x) * (float) frameSize.y),
        (float) frameSize.x,
        (float) frameSize.y
    };
}