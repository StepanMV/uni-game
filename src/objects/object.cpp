#include "object.h"
#include "tile.h"
#include "player.h"
#include "enemy.h"
#include "projectile.h"
#include "renderer.h"
#include <cmath>
#include "game.h"
#include <iostream>

std::vector<std::vector<std::shared_ptr<Tile>>> Object::tiles;
std::vector<std::shared_ptr<Object>> Object::objects;

unsigned Object::getId() const {
    return id;
}

Vec2 Object::getSpeed()
{
    return physics->speed;
}

Vec2 Object::getPos() const {
    return pos;
}

Vec2 Object::getSize() const {
    return size;
}

float Object::getAngle() const {
    return angle;
}

Object::Object(const Object &other)
{
    pos = other.pos;
    size = other.size;
    if (std::shared_ptr<CoolRenderer> rend = std::dynamic_pointer_cast<CoolRenderer>(other.renderer)) {
        this->renderer = std::make_shared<CoolRenderer>(*rend);
    } else if (std::shared_ptr<TileRenderer> rend = std::dynamic_pointer_cast<TileRenderer>(other.renderer)) {
        this->renderer = std::make_shared<TileRenderer>(*rend);
    }
    renderer->changeObject(&pos);
    if (other.physics) physics = std::make_shared<Physics>(*other.physics);
    hitbox = other.hitbox;
    angle = other.angle;
}

Object &Object::operator=(const Object &other) {
    pos = other.pos;
    size = other.size;
    if (std::shared_ptr<CoolRenderer> rend = std::dynamic_pointer_cast<CoolRenderer>(other.renderer)) {
        this->renderer = std::make_shared<CoolRenderer>(*rend);
    } else if (std::shared_ptr<TileRenderer> rend = std::dynamic_pointer_cast<TileRenderer>(other.renderer)) {
        this->renderer = std::make_shared<TileRenderer>(*rend);
    }
    renderer->changeObject(&pos);
    if (other.physics) physics = std::make_shared<Physics>(*other.physics);
    hitbox = other.hitbox;
    angle = other.angle;
    return *this;
}

void Object::updateAll() {
    objects.erase(std::remove_if(objects.begin(), objects.end(), [](std::shared_ptr<Object> object) {
        return !object->isAlive();
    }), objects.end());

    for (int i = 0; i < objects.size(); ++i) {
        objects[i]->pos += objects[i]->physics->calcSpeed();
        objects[i]->update();
        //if(object->physics) object->physics->move();

    }

    for(auto& object : objects) {
        //if(!object->collider) continue;
        for(int i = (object->getPos().y - object->getSize().y) / Level::tileSize; i < (object->getPos().y + object->getSize().y) / Level::tileSize; i++) {
            for(int j = (object->getPos().x - object->getSize().x) / Level::tileSize; j < (object->getPos().x + object->getSize().x) / Level::tileSize; j++) {
                if(i < 0 || j < 0 || i >= Level::height || j >= Level::width) continue;
                if(object->MyCheckCollision(std::dynamic_pointer_cast<Object>(tiles[i][j]))) {
                    object->onCollision(tiles[i][j]);
                }
            }
        }
    }

    for(auto& obj1 : objects) {
        //if(!obj1->collider) continue;
        for(auto& obj2 : objects) {
            //if(!obj2->collider) continue;
            if(obj1 == obj2) continue;
            if(obj1->checkCollision(obj2)) {
                if(auto ptr = std::dynamic_pointer_cast<Projectile>(obj2)) obj1->onCollision(ptr);
                if(auto ptr = std::dynamic_pointer_cast<Enemy>(obj2)) obj1->onCollision(ptr);
                if(auto ptr = std::dynamic_pointer_cast<Player>(obj2)) obj1->onCollision(ptr);
            }
        }
    }
}

void Object::renderAll() {
    for (auto& object : objects) {
        object->render();
        object->renderer->render();
    }
    auto borders = Game::camera->getRenderBounds();
    for(int i = borders.z; i < borders.w; ++i) {
        for (int j = borders.x; j < borders.y; ++j) {
            if (!tiles[i][j]->isAlive()) continue;
            tiles[i][j]->render();
            tiles[i][j]->renderer->render();
        }
    }
}

void Object::destroy() {
    id = 0;
}

bool Object::checkCollision(const std::shared_ptr<Object> other) const
{
    Rectangle thisHitbox = Rectangle{(float) (pos.x - size.x / 2), (float) (pos.y - size.y / 2), (float) size.x, (float) size.y};
    Vec2 otherPos = other->getPos();
    Vec2 otherSize = other->getSize();
    Rectangle otherHitbox = Rectangle{(float) (otherPos.x - otherSize.x / 2), (float) (otherPos.y - otherSize.y / 2), (float) otherSize.x, (float) otherSize.y};
    return CheckCollisionRecs(thisHitbox, otherHitbox);
}

std::vector<Vec2> getPointsOfRect(Vec2 pos, Vec2 size) {
    std::vector<Vec2> points;
    points.push_back(pos + Vec2(-size.x / 2, -size.y / 2));
    points.push_back(pos + Vec2(size.x / 2, -size.y / 2));
    points.push_back(pos + Vec2(size.x / 2, size.y / 2));
    points.push_back(pos + Vec2(-size.x / 2, size.y / 2));
    return points;
}

void Object::setCenterOffset(Vec2 offset) {
    centerOffset = offset;
}

bool Object::isAlive() const {
    return id != 0;
}

// void Object::calcHitbox() {
//     hitbox.clear();
//     hitbox.push_back(Vec2(+size.x / 2, +size.y / 2));
//     hitbox.push_back(Vec2(+size.x / 2, -size.y / 2));
//     hitbox.push_back(Vec2(-size.x / 2, -size.y / 2));
//     hitbox.push_back(Vec2(-size.x / 2, +size.y / 2));
//     Vec2 oldCenter = centerOffset;
//     Vec2 posOffset = -oldCenter;
//     posOffset.rotate(angle);
//     centerOffset = -posOffset;
//     posOffset += oldCenter;
//     for(auto& point : hitbox) {
//         point -= oldCenter;
//         point.rotate(angle);
//         point += oldCenter;
//         point += pos;
//     }
//     pos += posOffset;
// }

void Object::calcHitbox() {
    pos -= startCenter;
    startCenter = -centerOffset;
    startCenter.rotate(angle);
    startCenter += centerOffset;
    for(int i = 0; i < 4; i++) {
        hitbox[i] -= startHitbox[i];
    }
    startHitbox = {Vec2(+size.x / 2, +size.y / 2),
                   Vec2(+size.x / 2, -size.y / 2),
                   Vec2(-size.x / 2, -size.y / 2),
                   Vec2(-size.x / 2, +size.y / 2)};
    for(int i = 0; i < 4; i++) {
        startHitbox[i] -= centerOffset;
        startHitbox[i].rotate(angle);
        startHitbox[i] += centerOffset;
        hitbox[i] = startHitbox[i] + pos;
    }
    pos += startCenter;
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

Vec2 farthestPointInDirection(Circle circle, Vec2 direction) {
    float angle = atan2(direction.y, direction.x);
    return circle.pos + Vec2(cos(angle), sin(angle)) * circle.radius;
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

bool Object::MyCheckCollision(const std::shared_ptr<Object> other) const {
    if(!other->isCollideable() || !isCollideable()) {
        return false;
    }
    return calculate(hitbox, other->hitbox);
}