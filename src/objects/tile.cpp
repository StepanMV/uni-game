#include "tile.h"

Tile::Tile(Vec2 _pos, Vec2 _size): Object(_pos, _size) {}

Tile& Tile::spawn() {
    Object::spawn();
    renderer.saveSprite("testTile", "resources/sprites/test.png");
    renderer.setState("testTile");
    return *this;
}

void Tile::update() {

}

void Tile::onCollision(Tile* other) {

}

void Tile::onCollision(Entity* other) {

}