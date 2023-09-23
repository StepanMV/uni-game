#include "entity.h"

Entity::Entity(Vec2 _pos, Vec2 _size): Object(_pos, _size) {}

Entity& Entity::setMaxSpeeds(double move, double fall, double fly) {
    physics.setMaxSpeeds(move, fall, fly);
    return *this;
}

Entity& Entity::setForces(double gravity, double friction) {
    physics.setForces(gravity, friction);
    return *this;
}

void Entity::move() {
    pos += physics.speed;
}