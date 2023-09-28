#include "level.h"
#include "player.h"
#include <fstream>

//posX, posY, sizeX, sizeY, id, form, isUp, isDown, isLeft, isRight
Level::Level(): startRenderX(0), endRenderX(0), startRenderY(0), endRenderY(0) {
    tiles.resize(levelSizeY);
    for(int i = 0; i < levelSizeY; i++) {
        tiles[i].resize(levelSizeX);
        for(int j = 0; j < levelSizeX; j++) {
            tiles[i][j] = Tile(Vec2(i * tileSize, j * tileSize), Vec2(tileSize, tileSize)).setId(0).setForm(0);
        }
    }
}

void Level::setCamera(const Camera2D* camera) {
    this->camera = camera;
}

void Level::loadLevel(std::string filename) {
    this->filename = filename;
    std::ifstream inf(filename);
    if(!inf) {
        //error
        return;
    }
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

void Level::saveLevel(std::string filename) {
    std::ofstream outf(filename);
    if(!outf) {
        //error
        return;
    }
    for (int i = 0; i < levelSizeY; i++) {
        for (int j = 0; j < levelSizeX; j++) {
            if (tiles[i][j].getId() != 0) {
                outf << tiles[i][j].getPos().x << " " 
                << tiles[i][j].getPos().y << " " 
                << tiles[i][j].getSize().x << " " 
                << tiles[i][j].getSize().y << " " 
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
    Vector2 pos = GetScreenToWorld2D(tile.getPos().toRaylib(), *camera);
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
    Vector2 pos = GetScreenToWorld2D(tile.getPos().toRaylib(), *camera);
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
    startRenderY = (camera->target.y - camera->offset.y / camera->zoom) / tileSize - 5;
    endRenderY = (camera->target.y + camera->offset.y / camera->zoom) / tileSize + 5;
    startRenderX = (camera->target.x - camera->offset.x / camera->zoom) / tileSize - 5;
    endRenderX = (camera->target.x + camera->offset.x / camera->zoom) / tileSize + 5;
}

void Level::render() {
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
    for(int i = startRenderY; i < endRenderY; i++) {
        for(int j = startRenderX; j < endRenderX; j++) {
            if(tiles[i][j].getId() != 0) {
                tiles[i][j].render();
            }
        }
    }
}

void Level::checkCollision(Player& pl) {
    for(int i = startRenderY; i < endRenderY; i++) {
        for(int j = startRenderX; j < endRenderX; j++) {
            if(pl.checkCollision(tiles[i][j])) {
                pl.onCollision(&tiles[i][j]);
            }
        }
    }
}

Level::~Level() {
    saveLevel(filename);
}