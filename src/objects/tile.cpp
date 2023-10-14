#include "tile.h"

Tile::Tile() {
    this->renderer = std::make_shared<TileRenderer>(&this->pos);
}

unsigned Tile::getId() const
{
    return id;
}

unsigned Tile::getForm() const {
    return form;
}

void Tile::onCollision(std::shared_ptr<Tile> other) {

}

void Tile::onCollision(std::shared_ptr<Entity> other) {
    
}

void Tile::onCollision(std::shared_ptr<Projectile> other) {
    
}

bool Tile::isAlive() const {
    return id != 0;
}

bool Tile::isCollideable() const {
    return id != 0;
}

void Tile::breakObject() {
    id = 0;
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

void Tile::render() {
    renderer->render();
}

TileBuilder TileBuilder::spawn(Vec2 pos, Vec2 size) {
    TileBuilder builder;
    builder.tile.pos = pos;
    builder.tile.size = size;
    builder.tile.renderer = std::make_shared<TileRenderer>(&builder.tile.pos);
    return builder;
}

TileBuilder &TileBuilder::setNeighbors(bool up, bool down, bool left, bool right) {
    tile.isUp = up;
    tile.isDown = down;
    tile.isLeft = left;
    tile.isRight = right;
    return *this;
}

TileBuilder &TileBuilder::setClimb(bool left, bool right) {
    tile.canClimbLeft = left;
    tile.canClimbRight = right;
    return *this;
}

TileBuilder &TileBuilder::setID(unsigned id) {
    tile.id = id;
    return *this;
}

TileBuilder &TileBuilder::setForm(unsigned form) {
    tile.form = form;
    return *this;
}

Tile TileBuilder::build() {
    if (tile.id == 0) return Tile();
    tile.isPlatform = tile.id == 1;
    auto renderer = std::dynamic_pointer_cast<TileRenderer>(tile.renderer);

    renderer->loadTexture("resources/textures/Tiles_" + std::to_string(tile.id) + ".png");
    tile.updateState();
    tile.calcHitbox();
    return tile;
}
