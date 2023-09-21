#include "tile.h"

Tile& Tile::spawn(Vec2 pos, Vec2 size) {
    Object::spawn(pos, size);
    renderer.saveSprite("testTile", "resources/sprites/test.png");
    renderer.setState("testTile");
    return *this;
}

bool Tile::checkCollision(const Object& other) const {
    return false;
}

void Tile::update() {

}

void Tile::onCollision(Tile* other) {

}

void Tile::onCollision(Entity* other) {

}