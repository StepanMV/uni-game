#include "entity.h"

bool Entity::getAttacking() const {
    return isAttacking;
}

Entity::Entity() {
    this->renderer = std::make_shared<CoolRenderer>(&this->pos);
    this->physics = std::make_shared<Physics>();
}

bool Entity::isAlive() const
{
    return health > 0;
}
