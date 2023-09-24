#include "tile.h"

Tile::Tile(Vec2 pos, Vec2 size) : Object(pos, size) { }

Tile& Tile::spawn(Vec2 pos, Vec2 size) {
    Object::spawn(pos, size);
    renderer.loadTexture("testTile", "resources/sprites/block.png");
    renderer.setMain("testTile", RendererType::TEXTURE);
    return *this;
}

void Tile::update() {

}