#include "object.h"
#include "renderer.h"

Object::Object(Vec2 _pos, Vec2 _size): ICollidable(_pos, _size) {}

Object& Object::spawn() {
    this->renderer = Renderer(&this->pos, &this->size);
    this->physics = Physics(&pos);
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