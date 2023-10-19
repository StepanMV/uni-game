#include "level.h"
#include "tile.h"
#include "projectile.h"
#include "controls.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "particle.h"
#include "cool_camera.h"
#include <fstream>
#include <filesystem>
#include <iostream>

const unsigned Level::tileSize = 16;
const unsigned Level::width = 2100;
const unsigned Level::height = 1100;
const unsigned Level::borderOffset = 50;

//posX, posY, id, form, isUp, isDown, isLeft, isRight, canClimbLeft, canClimgRight
Level::Level() {
    Object::tiles.resize(height);
    for(int i = 0; i < height; i++) {
        Object::tiles[i].resize(width);
        for(int j = 0; j < width; j++) {
            Object::tiles[i][j] = TileBuilder::spawn(0, {(j + 0.5f) * tileSize, (i + 0.5f) * tileSize}, {tileSize, tileSize}).build();
        }
    }
}

Level::~Level() {
    if (loaded) save();
}

void Level::loadFile(std::string filepath) {
    this->filepath = filepath;
    std::ifstream inf(filepath);
    if(inf) {
        unsigned idY, idX;
        unsigned posX, posY, id, form;
        bool isUp, isDown, isLeft, isRight, canClimbLeft, canClimbRight;
        while(inf) {
            inf >> posX;
            if(!inf) {
                return;
            }
            inf >> posY >> id >> form >> isUp >> isDown >> isLeft >> isRight >> canClimbLeft >> canClimbRight;
            Object::tiles[posY / tileSize][posX / tileSize] = TileBuilder::spawn(id, {(float) posX, (float) posY}, {tileSize, tileSize})
                .setForm(form)
                .setNeighbors(isUp, isDown, isLeft, isRight)
                .setClimb(canClimbLeft, canClimbRight)
                .build();
        }
        inf.close();
    }
}

void Level::loadGame(std::string filename)
{
    loadFile(filename);
    player = PlayerBuilder::spawn(3, Vec2(500 * tileSize, (height - 100) * tileSize), Vec2(tileSize * 2, tileSize * 3))
        .setMaxSpeeds(10, 10, 8)
        .setForces(0.5, 0.75)
        .build();
    Game::camera->setTarget(&player->transform->pos);
    loaded = true;
    this->editor = false;
}

void Level::loadEditor(std::string filename) {
    loadFile(filename);

    player = PlayerBuilder::spawn(1, Vec2(500 * tileSize, (height - 100) * tileSize), Vec2(tileSize * 2, tileSize * 3))
        .setMaxSpeeds(10, 10, 8)
        .setForces(0.5, 0)
        .build();
    Game::camera->setTarget(&player->transform->pos);
    loaded = true;
    this->editor = true;
}

void Level::save() {
    Object::objects.clear();
    std::string dirpath = filepath.substr(0, filepath.rfind('/'));
    std::filesystem::create_directories(dirpath);
    std::ofstream outf(filepath);
    if(!outf) {
        throw std::runtime_error("Open file error");
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (Object::tiles[i][j]->getId() != 0) {
                outf << Object::tiles[i][j]->getPos().x << " " 
                << Object::tiles[i][j]->getPos().y << " " 
                << Object::tiles[i][j]->getId() << " " 
                << Object::tiles[i][j]->getForm() << " "
                << Object::tiles[i][j]->isUp << " " 
                << Object::tiles[i][j]->isDown << " " 
                << Object::tiles[i][j]->isLeft << " " 
                << Object::tiles[i][j]->isRight << " " 
                << Object::tiles[i][j]->canClimbLeft << " "
                << Object::tiles[i][j]->canClimbRight << std::endl;
            }
        }
    }
    outf.close();
}

bool Level::isTile(Vector2 pos) const {
    return Object::tiles[pos.y / tileSize][pos.x / tileSize]->getId() != 0;
}

void Level::setClimb(unsigned idY, unsigned idX) {
    if(idY >= 3) {
        Object::tiles[idY][idX]->canClimbLeft = (!(Object::tiles[idY - 1][idX]->getId() && !Object::tiles[idY - 1][idX]->isPlatform)
        && !(Object::tiles[idY - 2][idX]->getId() && !Object::tiles[idY - 2][idX]->isPlatform)
        && !(Object::tiles[idY - 3][idX]->getId() && !Object::tiles[idY - 3][idX]->isPlatform)
        && !(Object::tiles[idY - 3][idX - 1]->getId() && !Object::tiles[idY - 3][idX - 1]->isPlatform)
        && idY > Level::borderOffset + 2);
        Object::tiles[idY][idX]->canClimbRight = (!(Object::tiles[idY - 1][idX]->getId() && !Object::tiles[idY - 1][idX]->isPlatform) 
        && !(Object::tiles[idY - 2][idX]->getId() && !Object::tiles[idY - 2][idX]->isPlatform) 
        && !(Object::tiles[idY - 3][idX]->getId() && !Object::tiles[idY - 3][idX]->isPlatform) 
        && !(Object::tiles[idY - 3][idX + 1]->getId() && !Object::tiles[idY - 3][idX + 1]->isPlatform) 
        && idY > Level::borderOffset + 2);
        }
}

void Level::setLocalPos(unsigned& idY, unsigned& idX, bool isAdded) {
    if(Object::tiles[idY][idX]->isPlatform) {
        Object::tiles[idY][idX]->isRight = isAdded && Object::tiles[idY][idX + 1]->getId();
        Object::tiles[idY][idX]->isLeft = isAdded && Object::tiles[idY][idX - 1]->getId();
        Object::tiles[idY][idX - 1]->isRight = isAdded && Object::tiles[idY][idX - 1]->isPlatform;
        Object::tiles[idY][idX + 1]->isLeft = isAdded && Object::tiles[idY][idX + 1]->isPlatform;
        Object::tiles[idY][idX - 1]->updateState();
        Object::tiles[idY][idX + 1]->updateState();
    }
    else {
        if(Object::tiles[idY - 1][idX]->getId() && !Object::tiles[idY - 1][idX]->isPlatform) {
            Object::tiles[idY - 1][idX]->isDown = isAdded;
            Object::tiles[idY][idX]->isUp = isAdded;
            Object::tiles[idY - 1][idX]->updateState();
        }
        if(Object::tiles[idY + 1][idX]->getId() && !Object::tiles[idY + 1][idX]->isPlatform) {
            Object::tiles[idY + 1][idX]->isUp = isAdded;
            Object::tiles[idY][idX]->isDown = isAdded;
            Object::tiles[idY + 1][idX]->updateState();
        }
        if(Object::tiles[idY][idX - 1]->getId()) {
            Object::tiles[idY][idX - 1]->isRight = isAdded;
            Object::tiles[idY][idX]->isLeft = isAdded && !Object::tiles[idY][idX - 1]->isPlatform;
            Object::tiles[idY][idX - 1]->updateState();
        }
        if(Object::tiles[idY][idX + 1]->getId()) {
            Object::tiles[idY][idX + 1]->isLeft = isAdded;
            Object::tiles[idY][idX]->isRight = isAdded && !Object::tiles[idY][idX + 1]->isPlatform;
            Object::tiles[idY][idX + 1]->updateState();
        }
    }
    Object::tiles[idY][idX]->updateState();

    if(isAdded) {
        setClimb(idY, idX);
    }

    setClimb(idY + 1, idX);
    setClimb(idY + 2, idX);
    setClimb(idY + 3, idX);
    setClimb(idY + 3, idX - 1);
    setClimb(idY + 3, idX + 1);
}

bool Level::isLoaded() const {
    return loaded;
}

void Level::unload() {
    loaded = false;
}

void Level::placeTile(const Vec2 tilePos, int id) {
    unsigned idX = tilePos.x / tileSize;
    unsigned idY = tilePos.y / tileSize;

    if(idX < 1 || idY < 1 || idX > width - 1 || idY > height - 1) return;
    if(isTile(tilePos.toRaylib())) return;

    Object::tiles[idY][idX] = TileBuilder::spawn(id, {(idX + 0.5f) * tileSize, (idY + 0.5f) * tileSize}, {tileSize, tileSize}).build();
    setLocalPos(idY, idX, true);
}

void Level::breakTile(const Vec2 tilePos) {
    unsigned idX = tilePos.x / tileSize;
    unsigned idY = tilePos.y / tileSize;

    if(idX < 1 || idY < 1 || idX > width - 1 || idY > height - 1) return;
    if(!isTile(tilePos.toRaylib())) return;

    Object::tiles[idY][idX]->destroy();
    setLocalPos(idY, idX, false);
}

void Level::render() {
    BeginMode2D(Game::camera->getCamera());
    Object::renderAll();
    EndMode2D();

    DrawText(std::to_string(player->getPos().x / tileSize).c_str(), 10, 40, 20, RED);
    DrawText(std::to_string(player->getPos().y / tileSize).c_str(), 10, 70, 20, RED);
}

void Level::update() {  

    if (editor) updateEditor();

    Object::updateAll();
    Vec2 playerSpeed = player->getSpeed();
    Game::background->setSpeed(0.2 * playerSpeed);
}

void Level::updateEditor() {
    Vector2 mousePos = GetScreenToWorld2D({(float) GetMouseX(), (float) GetMouseY()}, Game::camera->getCamera());
    Vec2 mp = {mousePos.x, mousePos.y};
    for (int i = 1; i <= 94; ++i) {
        if (Game::ui->getSubUI("tileSelector")->isButtonHeld("tile_" + std::to_string(i))) {
            placedBlockId = i;
            break;
        }
    }
    if (Controls::isMouseDown(MOUSE_BUTTON_LEFT)) placeTile(mp, placedBlockId);
    if (Controls::isMouseDown(MOUSE_BUTTON_RIGHT)) breakTile(mp);

}