#include "level.h"
#include <fstream>

//posX, posY, sizeX, sizeY, id, form
#include <iostream>
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
    std::ifstream inf(filename);
    if(!inf) {
        //error
        return;
    }
    int posX, posY, sizeX, sizeY, id, form;
    while(inf) {
        inf >> posX >> posY >> sizeX >> sizeY >> id >> form;
        tiles[posY / tileSize][posX / tileSize] = Tile(Vec2(posX, posY), Vec2(sizeX, sizeY)).setId(id).setForm(form);
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
                << tiles[i][j].getForm() << std::endl;
            }
        }
    }
    outf.close();
}

bool Level::isTile(const Tile& tile) const {
    return tiles[tile.getPos().y / tileSize][tile.getPos().x / tileSize].getId() != 0;
}

void Level::placeTile(const Tile& tile) {//&?
    if(isTile(tile)) {
        return;
    }
    Vector2 pos = GetScreenToWorld2D(tile.getPos().toRaylib(), *camera);
    if(pos.x < 0 || pos.y < 0 || pos.x > levelSizeX * tileSize || pos.y > levelSizeY * tileSize) {
        return;
    }
    tiles[pos.y / tileSize][pos.x / tileSize] = tile;
    tiles[pos.y / tileSize][pos.x / tileSize].setId(1).spawn(Vec2(pos.x - (int)pos.x % tileSize, pos.y - (int)pos.y % tileSize), tile.getSize());
}

void Level::breakTile(const Tile& tile) {
    if(!isTile(tile)) {
        return;
    }
    Vector2 pos = GetScreenToWorld2D(tile.getPos().toRaylib(), *camera);
    tiles[pos.y / tileSize][pos.x / tileSize] = Tile(Vec2(tile.getPos().x, tile.getPos().y), Vec2(tileSize, tileSize)).setId(0).setForm(0);
    //tiles[tile.getPos().x / tileSize][tile.getPos().y / tileSize] = Tile(Vec2(tile.getPos().x, tile.getPos().y), Vec2(tileSize, tileSize)).setId(0).setForm(0);
}

void Level::calcCords() {
    startRenderY = (camera->target.y - 2 * camera->offset.y) / tileSize;
    endRenderY = (camera->target.y + 2 * camera->offset.y) / tileSize;
    startRenderX = (camera->target.x - 2 * camera->offset.x) / tileSize;
    endRenderX = (camera->target.x + 2 * camera->offset.x) / tileSize;
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

Level::~Level() {
    //saveLevel("resources/levels/level.txt");
}