#include "collider.h"
#include "object.h"

ICollidable::ICollidable(Vec2 _pos, Vec2 _size): pos(_pos), size(_size) {}

Rectangle recToHitbox(const Vec2& pos, const Vec2& size) {
    return Rectangle{static_cast<float>(pos.x - size.x / 2), static_cast<float>(pos.y - size.y / 2), static_cast<float>(size.x), static_cast<float>(size.y)};
}

bool ICollidable::checkCollision(const Object& other) const {
    Rectangle thisHitbox = recToHitbox(pos, size);
    Rectangle otherHitbox = recToHitbox(other.getPos(), other.getSize());
    DrawRectangleRec(thisHitbox, RED);
    DrawRectangleRec(otherHitbox, GREEN);
    return CheckCollisionRecs(thisHitbox, otherHitbox);
}



Rectangle ICollidable::recCollision(const Object& other) const {    
    Rectangle thisHitbox = recToHitbox(pos, size);
    Rectangle otherHitbox = recToHitbox(other.getPos(), other.getSize());
    return GetCollisionRec(thisHitbox, otherHitbox);
}

Rectangle ICollidable::recCollision(const Vec2& otherPos, const Vec2& otherSize) const {
    Rectangle thisHitbox = recToHitbox(pos, size);
    Rectangle otherHitbox = recToHitbox(otherPos, otherSize);
    return GetCollisionRec(thisHitbox, otherHitbox);
}