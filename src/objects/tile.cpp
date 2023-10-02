#include "tile.h"

unsigned Tile::getId() const {
    return id;
}

unsigned Tile::getForm() const {
    return form;
}

void Tile::update() { }

void Tile::render() {
    renderer.update(pos, size);
    renderer.render();
}

TileBuilder TileBuilder::spawn(Vec2 pos, Vec2 size) {
    TileBuilder builder;
    builder.pos = pos;
    builder.size = size;
    return builder;
}

TileBuilder &TileBuilder::setNeighbors(bool up, bool down, bool left, bool right) {
    this->up = up;
    this->down = down;
    this->left = left;
    this->right = right;
    return *this;
}

TileBuilder &TileBuilder::setID(unsigned id) {
    this->id = id;
    return *this;
}

TileBuilder &TileBuilder::setForm(unsigned form) {
    this->form = form;
    return *this;
}

Tile TileBuilder::build() const {
    if (id == 0) return Tile();
    Tile tile;
    tile.pos = std::move(pos);
    tile.size = std::move(size);

    tile.id = id;
    tile.form = form;
    tile.isUp = up;
    tile.isDown = down;
    tile.isLeft = left;
    tile.isRight = right;

    tile.renderer.loadTexture("testTile", "resources/textures/Tiles_" + std::to_string(id) + ".png");
    tile.renderer.addToState("main", "testTile").spriteSheet({16, 15}, {1, 1});
    tile.renderer.setState("main");
    return tile;
}
