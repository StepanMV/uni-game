#include "tile.h"

Tile::Tile() {
    this->renderer = std::make_shared<TileRenderer>(&this->pos);
}

unsigned Tile::getForm() const {
    return form;
}

void Tile::onCollision(std::shared_ptr<Tile> other) {

}

void Tile::onCollision(std::shared_ptr<Enemy> other) {
    
}

void Tile::onCollision(std::shared_ptr<Projectile> other) {
    
}

void Tile::onCollision(std::shared_ptr<Player> other) {
    
}

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
    renderer->setSpritePos(state);
}

void Tile::update() { }

void Tile::render() { }

TileBuilder TileBuilder::spawn(unsigned id, Vec2 pos, Vec2 size) {
    TileBuilder builder;
    builder.tile = std::shared_ptr<Tile>(new Tile());
    builder.tile->id = id;
    builder.tile->pos = pos;
    builder.tile->size = size;
    builder.tile->renderer = std::make_shared<TileRenderer>(&builder.tile->pos);
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

TileBuilder &TileBuilder::setForm(unsigned form) {
    tile->form = form;
    return *this;
}

std::shared_ptr<Tile> TileBuilder::build() {
    if (tile->id == 0) return tile;
    tile->isPlatform = tile->id == 1;
    auto renderer = std::dynamic_pointer_cast<TileRenderer>(tile->renderer);

    renderer->loadTexture("resources/textures/Tiles_" + std::to_string(tile->id) + ".png");
    tile->updateState();
    tile->collider->calcHitbox();
    return tile;
}
