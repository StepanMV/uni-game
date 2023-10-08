#include "entity.h"

bool Entity::getAttacking() const {
    return isAttacking;
}

bool Entity::isAlive() const {
    return health > 0;
}

Entity::Entity(const Entity &other) {
    pos = other.pos;
    size = other.size;
    renderer = std::make_shared<CoolRenderer>(*std::dynamic_pointer_cast<CoolRenderer>(other.renderer));
    renderer->changeObject(&pos);
    physics = std::make_shared<Physics>(*other.physics);
    max_health = other.max_health;
    health = other.health;
}

Entity &Entity::operator=(const Entity &other) {
    pos = other.pos;
    size = other.size;
    renderer = std::make_shared<CoolRenderer>(*std::dynamic_pointer_cast<CoolRenderer>(other.renderer));
    renderer->changeObject(&pos);
    physics = std::make_shared<Physics>(*other.physics);
    max_health = other.max_health;
    health = other.health;
    return *this;
}
