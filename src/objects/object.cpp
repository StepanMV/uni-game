#include "object.h"
#include "tile.h"
#include "player.h"
#include "enemy.h"
#include "entity.h"
#include "particle.h"
#include "projectile.h"
#include "renderer.h"
#include <cmath>
#include "game.h"
#include <iostream>

std::vector<std::shared_ptr<Projectile>> Object::projectiles;
std::vector<std::shared_ptr<Enemy>> Object::enemies;
std::vector<std::vector<std::shared_ptr<Tile>>> Object::tiles;
std::vector<std::shared_ptr<Particle>> Object::particles;
std::shared_ptr<Player> Object::player;

unsigned Object::getId() const {
    return id;
}

Vec2 Object::getSpeed() const
{
    return physics->speed;
}

Vec2 Object::getPos() const {
    return transform->pos;
}

Vec2 Object::getSize() const {
    return transform->size;
}

float Object::getAngle() const {
    return transform->angle;
}

Vec2 Object::getCenterOffset() const {
    return collider->centerOffset;
}

void Object::setState(std::string state) {
    std::dynamic_pointer_cast<CoolRenderer>(renderer)->setState(state);
}

Object::Object(const Object &other)
{
    transform->pos = other.transform->pos;
    transform->size = other.transform->size;
    if (std::shared_ptr<CoolRenderer> rend = std::dynamic_pointer_cast<CoolRenderer>(other.renderer)) {
        this->renderer = std::make_shared<CoolRenderer>(*rend);
    } else if (std::shared_ptr<TileRenderer> rend = std::dynamic_pointer_cast<TileRenderer>(other.renderer)) {
        this->renderer = std::make_shared<TileRenderer>(*rend);
    }
    renderer->changeObject(transform);
    if (other.physics) physics = std::make_shared<Physics>(*other.physics);
    if (other.collider) collider = std::make_shared<Collider>(*other.collider);
    transform->angle = other.transform->angle;
}

Object &Object::operator=(const Object &other) {
    transform->pos = other.transform->pos;
    transform->size = other.transform->size;
    if (std::shared_ptr<CoolRenderer> rend = std::dynamic_pointer_cast<CoolRenderer>(other.renderer)) {
        this->renderer = std::make_shared<CoolRenderer>(*rend);
    } else if (std::shared_ptr<TileRenderer> rend = std::dynamic_pointer_cast<TileRenderer>(other.renderer)) {
        this->renderer = std::make_shared<TileRenderer>(*rend);
    }
    renderer->changeObject(transform);
    if (other.physics) physics = std::make_shared<Physics>(*other.physics);
    if (other.collider) collider = std::make_shared<Collider>(*other.collider);
    transform->angle = other.transform->angle;
    return *this;
}

void Object::clearObjects() {
    projectiles.clear();
    enemies.clear();
    player = nullptr;
}

void Object::addProjectile(std::shared_ptr<Projectile> projectile) {
    projectiles.push_back(projectile);
}

void Object::addEnemy(std::shared_ptr<Enemy> enemy) {
    enemies.push_back(enemy);
}

void Object::addPlayer(std::shared_ptr<Player> player) {
    Object::player = player;
}

void Object::addParticle(std::shared_ptr<Particle> particle) {
    particles.push_back(particle);
}

void Object::updateEnemies() {
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](std::shared_ptr<Enemy> enemy) {
        return !enemy->isAlive();
    }), enemies.end());

    for (int i = 0; i < enemies.size(); ++i) {
        enemies[i]->transform->pos += enemies[i]->physics->calcSpeed();
        enemies[i]->update();
    }

    for(auto& enemy : enemies) {
        if(!enemy->collider || !enemy->tileCollide) continue;
        for(int i = (enemy->getPos().y - enemy->getSize().y) / Level::tileSize; i < (enemy->getPos().y + enemy->getSize().y) / Level::tileSize; i++) {
            for(int j = (enemy->getPos().x - enemy->getSize().x) / Level::tileSize; j < (enemy->getPos().x + enemy->getSize().x) / Level::tileSize; j++) {
                if(i < 0 || j < 0 || i >= Level::height || j >= Level::width) continue;
                if(enemy->collider->MyCheckCollision(std::dynamic_pointer_cast<Object>(tiles[i][j])->collider)) {
                    auto entity = std::dynamic_pointer_cast<Entity>(enemy);
                    entity->onCollision(tiles[i][j]);
                }
            }
        }
    }
}

void Object::renderEnemies() {
    for (auto& enemy : enemies) {
        enemy->render();
        enemy->renderer->render();
    }
}

void Object::updateProjectiles() {
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [](std::shared_ptr<Projectile> projectile) {
        return !projectile->isAlive();
    }), projectiles.end());

    for (int i = 0; i < projectiles.size(); ++i) {
        projectiles[i]->transform->pos += projectiles[i]->physics->calcSpeed();
        projectiles[i]->update();
    }

    for(auto& proj : projectiles) {
        if(!proj->collider || !proj->tileCollide) continue;
        for(int i = (proj->getPos().y - proj->getSize().y) / Level::tileSize; i < (proj->getPos().y + proj->getSize().y) / Level::tileSize; i++) {
            for(int j = (proj->getPos().x - proj->getSize().x) / Level::tileSize; j < (proj->getPos().x + proj->getSize().x) / Level::tileSize; j++) {
                if(i < 0 || j < 0 || i >= Level::height || j >= Level::width) continue;
                if(proj->collider->MyCheckCollision(std::dynamic_pointer_cast<Object>(tiles[i][j])->collider)) {
                    proj->onCollision(tiles[i][j]);
                }
            }
        }
    }
}

void Object::renderProjectiles() {
    for (auto& proj : projectiles) {
        proj->render();
        proj->renderer->render();
    }
}

void Object::updateParticles() {
    particles.erase(std::remove_if(particles.begin(), particles.end(), [](std::shared_ptr<Particle> particles) {
    return !particles->isAlive();
    }), particles.end());

    for (int i = 0; i < particles.size(); ++i) {
        particles[i]->transform->pos += particles[i]->physics->calcSpeed();
        particles[i]->update();
    }
}

void Object::renderParticles() {
    for (auto& particles : particles) {
        particles->render();
        particles->renderer->render();
    }
}

void Object::updatePlayer() {
    if(!player->isAlive()) return;
    player->transform->pos += player->physics->calcSpeed();
    player->update();

    for(int i = (player->getPos().y - player->getSize().y) / Level::tileSize; i < (player->getPos().y + player->getSize().y) / Level::tileSize; i++) {
        for(int j = (player->getPos().x - player->getSize().x) / Level::tileSize; j < (player->getPos().x + player->getSize().x) / Level::tileSize; j++) {
            if(i < 0 || j < 0 || i >= Level::height || j >= Level::width) continue;
            if(player->collider->MyCheckCollision(std::dynamic_pointer_cast<Object>(tiles[i][j])->collider)) {
                auto entity = std::dynamic_pointer_cast<Entity>(player);
                entity->onCollision(tiles[i][j]);
            }
        }
    }
}

void Object::renderPlayer() {
    if(!player->isAlive()) return;
    player->render();
    player->renderer->render();
}

void Object::updateAll() {
    updatePlayer();
    updateEnemies();
    updateProjectiles();
    updateParticles();

    for(auto& enemy : enemies) {
        if(!enemy->collider || !enemy->objectCollide) continue;
        if(player->isAlive() && player->collider->MyCheckCollision(enemy->collider)) {
            player->onCollision(enemy);
        }
        for(auto& proj : projectiles) {
            if(!proj->collider || !proj->objectCollide) continue;
            if(!proj->getFromPlayer()) continue;
            if(enemy->collider->MyCheckCollision(proj->collider)) {
                enemy->onCollision(proj);
                proj->onCollision(enemy);
            }
        }
    }
    if(!player->isAlive()) return;
    for(auto& proj : projectiles) {
        if(!proj->collider || !proj->objectCollide) continue;
        if(proj->getFromPlayer()) continue;
        if(player->collider->MyCheckCollision(proj->collider)) {
            player->onCollision(proj);
            proj->onCollision(player);
        }
    }
}

void Object::renderAll() {
    renderEnemies();
    renderProjectiles();
    renderParticles();
    renderPlayer();
    auto borders = Level::camera->getRenderBounds();
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

bool Object::isAlive() const {
    return id != 0;
}