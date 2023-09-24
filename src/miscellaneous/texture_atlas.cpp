#include "texture_atlas.h"

#include "raylib.h"
#include "vec2.h"

TextureAtlas::TextureAtlas(std::shared_ptr<Texture2D> texture, Vec2 frameSize)
    :
    texture(texture),
    frameSize(frameSize),
    size(Vec2((int) (texture->width / size.x), (int) (texture->height / size.y)))
    { }

Rectangle TextureAtlas::getRect(Vec2 pos) {
    return Rectangle {
        (float) (((int) pos.x % (int) size.x) * (float) frameSize.x),
        (float) (((int) pos.y % (int) size.y) * (float) frameSize.y),
        (float) frameSize.x,
        (float) frameSize.y
    };
}