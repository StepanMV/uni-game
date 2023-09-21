#include "entity.h"

Entity& Entity::spawn(Vec2 pos, Vec2 size) {
    Object::spawn(pos, size);
    physics = Physics(&this->pos);
    return *this;
}

Entity& Entity::setMaxSpeeds(double move, double fall, double fly) {
    physics.setMaxSpeeds(move, fall, fly);
    return *this;
}

Entity& Entity::setForces(double gravity, double friction) {
    physics.setForces(gravity, friction);
    return *this;
}

bool Entity::checkCollision(const Object& other) const {
    Rectangle thisHitbox = Rectangle{(float) (pos.x - size.x / 2), (float) (pos.y - size.y / 2), (float) size.x, (float) size.y};
    Vec2 otherPos = other.getPos();
    Vec2 otherSize = other.getSize();
    Rectangle otherHitbox = Rectangle{(float) (otherPos.x - otherSize.x / 2), (float) (otherPos.y - otherSize.y / 2), (float) otherSize.x, (float) otherSize.y};
    return CheckCollisionRecs(thisHitbox, otherHitbox);
}

void Entity::move() {
    pos += physics.speed;
}