#include "object.h"
#include "renderer.h"

Object& Object::spawn(Vec2 pos, Vec2 size) {
    this->pos = pos;
    this->size = size;
    this->renderer = Renderer(&this->pos, &this->size);
    return *this;
}

Vec2 Object::getPos() const {
    return pos;
}

Vec2 Object::getSize() const {
    return size;
}

void Object::render() {
    renderer.update();
}