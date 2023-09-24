#include "object.h"
#include "renderer.h"

Object::Object(Vec2 pos, Vec2 size) : pos(pos), size(size), renderer(&this->pos, &this->size) { }

Object::Object(const Object& other) : pos(other.pos), size(other.size)
    {
        this->renderer = other.renderer;
        this->renderer.changeObject(&this->pos, &this->size);
    }

Object& Object::operator=(const Object& other) {
    pos = other.pos;
    size = other.size;
    renderer = other.renderer;
    renderer.changeObject(&this->pos, &this->size);
    return *this;
}

Object::Object(Object&& other) : pos(other.pos), size(other.size), renderer(&this->pos, &this->size) {
    this->renderer = other.renderer;
    other.pos = Vec2();
    other.size = Vec2();
    other.renderer = Renderer();
}

Object& Object::spawn(Vec2 pos, Vec2 size) {
    this->pos = pos;
    this->size = size;
    this->renderer = Renderer(&this->pos, &this->size);
    return *this;
}

Object& Object::setMaxSpeeds(double move, double fall, double fly) {
    physics.setMaxSpeeds(move, fall, fly);
    return *this;
}

Object& Object::setForces(double gravity, double friction) {
    physics.setForces(gravity, friction);
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

void Object::move() {
    pos += physics.speed;
}

bool Object::checkCollision(const Object& other) const {
    Rectangle thisHitbox = Rectangle{(float) (pos.x - size.x / 2), (float) (pos.y - size.y / 2), (float) size.x, (float) size.y};
    Vec2 otherPos = other.getPos();
    Vec2 otherSize = other.getSize();
    Rectangle otherHitbox = Rectangle{(float) (otherPos.x - otherSize.x / 2), (float) (otherPos.y - otherSize.y / 2), (float) otherSize.x, (float) otherSize.y};
    return CheckCollisionRecs(thisHitbox, otherHitbox);
}

Rectangle Object::getCollisionBox(const Object& other) const {
    Rectangle thisHitbox = Rectangle{(float) (pos.x - size.x / 2), (float) (pos.y - size.y / 2), (float) size.x, (float) size.y};
    Vec2 otherPos = other.getPos();
    Vec2 otherSize = other.getSize();
    Rectangle otherHitbox = Rectangle{(float) (otherPos.x - otherSize.x / 2), (float) (otherPos.y - otherSize.y / 2), (float) otherSize.x, (float) otherSize.y};
    return GetCollisionRec(thisHitbox, otherHitbox);
}