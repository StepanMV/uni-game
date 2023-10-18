#include "entity.h"

bool Entity::getAttacking() const {
    return isAttacking;
}

Entity::Entity() {
    this->renderer = std::make_shared<CoolRenderer>(transform);
    this->physics = std::make_shared<Physics>();
}
