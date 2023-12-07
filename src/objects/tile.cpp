#include "tile.h"
#include "audio.h"

bool Tile::isCollideable() const {
    return id != 0;
}

void Tile::updateState() {
    auto renderer = std::dynamic_pointer_cast<TileRenderer>(this->renderer);
    unsigned short int state = 0;
    if (isUp) state += 8;
    if (isDown) state += 4;
    if (isLeft) state += 2;
    if (isRight) state += 1;
    if (isPlatform) renderer->setSpritePos(state, PLATFORM_TILE);
    else renderer->setSpritePos(state, DEFAULT_TILE);
}

void Tile::destroy() {
    id = 0;
    collider = nullptr;
    
}

TileBuilder TileBuilder::spawn(unsigned id, Vec2 pos, Vec2 size) {
    TileBuilder builder;
    builder.tile = std::shared_ptr<Tile>(new Tile());
    builder.tile->id = id;
    builder.tile->transform->pos = pos;
    builder.tile->transform->size = size;
    builder.tile->renderer = std::make_shared<TileRenderer>(builder.tile->transform);
    return builder;
}

TileBuilder &TileBuilder::setNeighbors(bool up, bool down, bool left, bool right) {
    tile->isUp = up;
    tile->isDown = down;
    tile->isLeft = left;
    tile->isRight = right;
    return *this;
}

TileBuilder &TileBuilder::setClimb(bool left, bool right) {
    tile->canClimbLeft = left;
    tile->canClimbRight = right;
    return *this;
}

std::shared_ptr<Tile> TileBuilder::build() {
    if (tile->id == 0) return tile;
    tile->collider = std::make_shared<Collider>(tile->transform);
    tile->isPlatform = tile->id == 2;
    auto renderer = std::dynamic_pointer_cast<TileRenderer>(tile->renderer);

    renderer->loadTexture("resources/textures/Tiles_" + std::to_string(tile->id) + ".png");
    tile->updateState();
    tile->collider->calcHitbox();
    return tile;
}
