#include "object.h"
#include "renderer.h"

Vec2 Object::getPos() const {
    return pos;
}

Vec2 Object::getSize() const {
    return size;
}

Vec2 Object::move()
{
    Vec2 speed = physics->calcSpeed();
    pos += speed;
    return speed;
}

bool Object::checkCollision(const Object &other) const
{
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
