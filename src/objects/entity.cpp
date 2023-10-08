#include "entity.h"

bool Entity::getAttacking() const {
    return isAttacking;
}

bool Entity::isAlive() const {
    return health > 0;
}