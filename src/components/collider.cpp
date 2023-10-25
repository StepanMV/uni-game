#include "collider.h"

Collider::Collider(std::shared_ptr<MyTransform> _transform) : transform(_transform) {
    calcHitbox();
}

void Collider::setCenterOffset(Vec2 offset) {
    centerOffset = offset;
}

void Collider::setScale(float scale) {
    transform->size *= (1 / this->transform->scale);
    transform->size *= scale;
    this->transform->scale = scale;
}

void Collider::setPos(Vec2 pos) {
    transform->pos = pos;
    transform->pos += rotatedCenter;
}

void Collider::calcHitbox() {
    transform->angle = (int)transform->angle % 360;
    transform->pos -= rotatedCenter;
    rotatedCenter = -centerOffset;
    rotatedCenter.rotate(transform->angle);
    rotatedCenter += centerOffset;
    for(int i = 0; i < 4; i++) {
        hitbox[i] -= rotatedHitbox[i];
    }
    rotatedHitbox = {
        Vec2(+transform->size.x / 2, +transform->size.y / 2),
        Vec2(+transform->size.x / 2, -transform->size.y / 2),
        Vec2(-transform->size.x / 2, -transform->size.y / 2),
        Vec2(-transform->size.x / 2, +transform->size.y / 2)
    };
    for(int i = 0; i < 4; i++) {
        rotatedHitbox[i] -= centerOffset;
        rotatedHitbox[i].rotate(transform->angle);
        rotatedHitbox[i] += centerOffset;
        hitbox[i] = rotatedHitbox[i] + transform->pos;
    }
    transform->pos += rotatedCenter;
}

//Collision GJK algorithm

Vec2 farthestPointInDirection(std::vector<Vec2> shape, Vec2 direction) {
    Vec2 farthestPoint = shape[0];
    for(Vec2 point : shape) {
        if(point.dot(direction) > farthestPoint.dot(direction)) {
            farthestPoint = point;
        }
    }
    return farthestPoint;
}

Vec2 support(std::vector<Vec2> shapeA, std::vector<Vec2> shapeB, Vec2 direction) {
    return farthestPointInDirection(shapeA, direction) - farthestPointInDirection(shapeB, -direction);
}

bool calculateDirection(std::vector<Vec2>& simplex, Vec2& direction) {
    Vec2 a = simplex[simplex.size() - 1];
    Vec2 ao = -a;
    if(simplex.size() == 3) {
        Vec2 b = simplex[1];
        Vec2 c = simplex[0];
        Vec2 ab = b - a;
        Vec2 ac = c - a;
        Vec2 abPerp = Vec2(-ab.y, ab.x);

        if(abPerp.dot(c) > 0) {
            abPerp = -abPerp;
        }
        if(abPerp.dot(ao) > 0) {
            simplex.erase(simplex.begin() + 1);
            direction = abPerp;
            return false;
        }

        Vec2 acPerp = Vec2(-ac.y, ac.x);
        if(acPerp.dot(b) > 0) {
            acPerp = -acPerp;
        }
        if(acPerp.dot(ao) > 0) {
            simplex.erase(simplex.begin());
            direction = acPerp;
            return false;
        }
        return true;
    } 
    else {
        Vec2 b = simplex[0];
        Vec2 ab = b - a;
        Vec2 abPerp = Vec2(-ab.y, ab.x);
        if(abPerp.dot(ao) < 0) {
            abPerp = -abPerp;
        }
        direction = abPerp;
        return false;
    }
}

bool calculate(std::vector<Vec2> shapeA, std::vector<Vec2> shapeB) {
    std::vector<Vec2> simplex;
    Vec2 direction = Vec2(1, 0);
    simplex.push_back(support(shapeA, shapeB, direction));
    direction = -direction;
    while(true) {
        simplex.push_back(support(shapeA, shapeB, direction));
        if(simplex[simplex.size() - 1].dot(direction) <= 0) {
            return false;
        }
        else {
            if(calculateDirection(simplex, direction)) {
                return true;
            }
        }
    }
}

bool Collider::MyCheckCollision(const std::shared_ptr<Collider> other) const {
    if(!other) {
        return false;
    }
    return calculate(hitbox, other->hitbox);
}