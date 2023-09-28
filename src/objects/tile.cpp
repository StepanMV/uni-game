#include "tile.h"

Tile::Tile(Vec2 pos, Vec2 size, bool _isUp, bool _isDown , bool _isLeft, bool _isRight) : 
Object(pos, size), 
isUp(_isUp), 
isDown(_isDown),
isLeft(_isLeft),
isRight(_isRight) { }

Tile& Tile::spawn(Vec2 pos, Vec2 size) {
    if(id == 0) {
        return *this;
    }
    Object::spawn(pos, size);
    renderer.loadTexture("testTile", "resources/sprites/block.png");
    renderer.setMain("testTile", RendererType::TEXTURE);
    return *this;
}

Tile& Tile::setId(unsigned long id) {
    this->id = id;
    return *this;
}

Tile& Tile::setForm(unsigned form) {
    this->form = form;
    return *this;
}

unsigned long Tile::getId() const {
    return id;
}

unsigned Tile::getForm() const {
    return form;
}

void Tile::update() {
}