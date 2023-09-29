#include "level.h"
#include "player.h"
#include <fstream>
#include <iostream>

const unsigned Level::tileSize = 64;
const unsigned Level::levelSizeX = 2000;
const unsigned Level::levelSizeY = 1000;

//posX, posY, sizeX, sizeY, id, form, isUp, isDown, isLeft, isRight
Level::Level() {
    tiles.resize(levelSizeY);
    for(int i = 0; i < levelSizeY; i++) {
        tiles[i].resize(levelSizeX);
        for(int j = 0; j < levelSizeX; j++) {
            tiles[i][j] = Tile(Vec2(i * tileSize, j * tileSize), Vec2(tileSize, tileSize)).setId(0).setForm(0);
        }
    }
}

void Level::load(std::string filename) {
    // Нужно убрать в отдельный метод
    // ==============================
    this->filename = filename;
    std::ifstream inf(filename);
    if(inf) {
        unsigned idY, idX;
        unsigned posX, posY, sizeX, sizeY, id, form;
        bool isUp, isDown, isLeft, isRight;
        while(inf) {
            inf >> posX >> posY >> sizeX >> sizeY >> id >> form >> isUp >> isDown >> isLeft >> isRight;
            tiles[posY / tileSize][posX / tileSize] = Tile(Vec2(posX, posY), Vec2(sizeX, sizeY), isUp, isDown, isLeft, isRight).setId(id).setForm(form);
            tiles[posY / tileSize][posX / tileSize].spawn(Vec2(posX, posY), Vec2(sizeX, sizeY));
        }
        inf.close();
    }
    // ==============================
    
    camera.offset = Vector2{GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    camera.target = Vector2{0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 0.25f;

	player.spawn(Vec2(500, 500), Vec2(tileSize * 2, tileSize * 3))
		.setMaxSpeeds(50, 30, 20)
		.setForces(0.75, 0.5);
}

void Level::save(std::string filename) {
    // тут должен быть код для создания папки и файла, если их нет
    std::ofstream outf(filename);
    if(!outf) {
        // лучше кидать exception, будет сразу понятно что что-то не так
        return;
    }
    for (int i = 0; i < levelSizeY; i++) {
        for (int j = 0; j < levelSizeX; j++) {
            if (tiles[i][j].getId() != 0) {
                outf << tiles[i][j].getPos().x << " " 
                << tiles[i][j].getPos().y << " " 
                << tiles[i][j].getSize().x << " " // зочем?
                << tiles[i][j].getSize().y << " "  // зочем?
                << tiles[i][j].getId() << " " 
                << tiles[i][j].getForm() << " "
                << tiles[i][j].isUp << " " 
                << tiles[i][j].isDown << " " 
                << tiles[i][j].isLeft << " " 
                << tiles[i][j].isRight << " " << std::endl;
            }
        }
    }
    outf.close();
}

bool Level::isTile(Vector2 pos) const {
    return tiles[pos.y / tileSize][pos.x / tileSize].getId() != 0;
}

void setLocalPos(std::vector<std::vector<Tile>>& tiles, unsigned& idY, unsigned& idX, bool isAdded) {
    if(tiles[idY - 1][idX].getId() != 0) {
        tiles[idY - 1][idX].isDown = isAdded;
        tiles[idY][idX].isUp = isAdded;
    }
    if(tiles[idY + 1][idX].getId() != 0) {
        tiles[idY + 1][idX].isUp = isAdded;
        tiles[idY][idX].isDown = isAdded;
    }
    if(tiles[idY][idX - 1].getId() != 0) {
        tiles[idY][idX - 1].isRight = isAdded;
        tiles[idY][idX].isLeft = isAdded;
    }
    if(tiles[idY][idX + 1].getId() != 0) {
        tiles[idY][idX + 1].isLeft = isAdded;
        tiles[idY][idX].isRight = isAdded;
    }
}

void Level::placeTile(const Tile& tile) {
    // Возможно проще будет преобразовать pos в позицию блока, а не в координаты
    // Условие длинновато
    Vector2 pos = GetScreenToWorld2D(tile.getPos().toRaylib(), camera);
    if(pos.x < tileSize || pos.y < tileSize || pos.x > (levelSizeX - 1) * tileSize || pos.y > (levelSizeY - 1) * tileSize) {
        return;
    }
    if(isTile(pos)) {
        return;
    }
    unsigned idX = pos.x / tileSize;
    unsigned idY = pos.y / tileSize;
    tiles[idY][idX] = tile;
    setLocalPos(tiles, idY, idX, true);
    tiles[idY][idX].setId(1).spawn(Vec2((int)pos.x - (int)pos.x % tileSize + tileSize / 2, (int)pos.y - (int)pos.y % tileSize + tileSize / 2), tile.getSize());
}

void Level::breakTile(const Tile& tile) {
    // Тоже
    Vector2 pos = GetScreenToWorld2D(tile.getPos().toRaylib(), camera);
    if(pos.x < tileSize || pos.y < tileSize || pos.x > (levelSizeX - 1) * tileSize || pos.y > (levelSizeY - 1) * tileSize) {
        return;
    }
    if(!isTile(pos)) {
        return;
    }
    unsigned idX = pos.x / tileSize;
    unsigned idY = pos.y / tileSize;
    setLocalPos(tiles, idY, idX, false);
    tiles[idY][idX].setId(0).setForm(0);
}

void Level::calcCords() {
    startRenderY = (camera.target.y - camera.offset.y / camera.zoom) / tileSize - 5;
    endRenderY = (camera.target.y + camera.offset.y / camera.zoom) / tileSize + 5;
    startRenderX = (camera.target.x - camera.offset.x / camera.zoom) / tileSize - 5;
    endRenderX = (camera.target.x + camera.offset.x / camera.zoom) / tileSize + 5;
}

void Level::render() {
    BeginMode2D(camera);
    
    this->calcCords();
    player.render();
    // Можно перенести в Level::calcCords()
    // ==============================
    if(startRenderX < 0) {
        startRenderX = 0;
    }
    if(startRenderY < 0) {
        startRenderY = 0;
    }
    if(endRenderX > levelSizeX) {
        endRenderX = levelSizeX;
    }
    if(endRenderY > levelSizeY) {
        endRenderY = levelSizeY;
    }
    // ==============================
    for(int i = startRenderY; i < endRenderY; i++) {
        for(int j = startRenderX; j < endRenderX; j++) {
            if(tiles[i][j].getId() != 0) {
                tiles[i][j].render();
            }
        }
    }

    EndMode2D();
}

void Level::update() {
	player.move();
    camera.target = player.getPos().toRaylib();
    player.update();
    this->checkCollision();
}

void Level::checkCollision() {
    for(int i = startRenderY; i < endRenderY; i++) {
        for(int j = startRenderX; j < endRenderX; j++) {
            if(player.checkCollision(tiles[i][j])) {
                player.onCollision(tiles[i][j]);
            }
        }
    }
}

Level::~Level() {
    save(filename);
}