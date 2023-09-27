#include "level.h"
#include "player.h"
#include <fstream>

//posX, posY, sizeX, sizeY, id, form
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
    double posX, posY, sizeX, sizeY, id, form;
    while(inf) {
        inf >> posX >> posY >> sizeX >> sizeY >> id >> form;
        tiles[posY / tileSize][posX / tileSize] = Tile(Vec2(posX, posY), Vec2(sizeX, sizeY)).setId(id).setForm(form);
        tiles[(int)posY / tileSize][(int)posX / tileSize].spawn(Vec2(posX, posY), Vec2(sizeX, sizeY));
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

void Level::placeTile(const Tile& tile) {
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
    startRenderY = (camera->target.y - camera->offset.y / camera->zoom) / tileSize;
    endRenderY = (camera->target.y + camera->offset.y / camera->zoom) / tileSize;
    startRenderX = (camera->target.x - camera->offset.x / camera->zoom) / tileSize;
    endRenderX = (camera->target.x + camera->offset.x / camera->zoom) / tileSize;
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
    //saveLevel("a.txt");
}