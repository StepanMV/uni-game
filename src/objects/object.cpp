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
    if (other.collider) collider = std::make_shared<Collider>(*other.collider);
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
    if (other.collider) collider = std::make_shared<Collider>(*other.collider);
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
        if(!object->collider || !object->tileCollide) continue;
        for(int i = (object->getPos().y - object->getSize().y) / Level::tileSize; i < (object->getPos().y + object->getSize().y) / Level::tileSize; i++) {
            for(int j = (object->getPos().x - object->getSize().x) / Level::tileSize; j < (object->getPos().x + object->getSize().x) / Level::tileSize; j++) {
                if(i < 0 || j < 0 || i >= Level::height || j >= Level::width) continue;
                if(object->collider->MyCheckCollision(std::dynamic_pointer_cast<Object>(tiles[i][j])->collider)) {
                    object->onCollision(tiles[i][j]);
                }
            }
        }
    }

    for(auto& obj1 : objects) {
        if(!obj1->collider || !obj1->objectCollide) continue;
        for(auto& obj2 : objects) {
            if(!obj2->collider || !obj2->objectCollide) continue;
            if(obj1 == obj2) continue;
            if(obj1->collider->MyCheckCollision(obj2->collider)) {
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

bool Object::isAlive() const {
    return id != 0;
}