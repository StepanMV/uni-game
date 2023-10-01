#include "level.h"
#include "player.h"
#include <fstream>
#include <filesystem>

const unsigned Level::tileSize = 16;
const unsigned Level::levelSizeX = 2100;
const unsigned Level::levelSizeY = 1100;
const unsigned Level::levelOffset = 50;

//posX, posY, id, form, isUp, isDown, isLeft, isRight, canClimbLeft, canClimgRight
Level::Level() {
    tiles.resize(levelSizeY);
    for(int i = 0; i < levelSizeY; i++) {
        tiles[i].resize(levelSizeX);
        for(int j = 0; j < levelSizeX; j++) {
            tiles[i][j] = Tile(Vec2(i * tileSize, j * tileSize), Vec2(tileSize, tileSize)).setId(0).setForm(0);
        }
    }
}

void Level::loadLevelFile(std::string filename) {
    this->filename = filename;
    std::ifstream inf(filename);
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
            tiles[posY / tileSize][posX / tileSize] = Tile(Vec2(posX, posY), Vec2(tileSize, tileSize), isUp, isDown, isLeft, isRight, canClimbLeft, canClimbRight).setId(id).setForm(form);
            tiles[posY / tileSize][posX / tileSize].spawn(Vec2(posX, posY), Vec2(tileSize, tileSize));
        }
        inf.close();
    }
}

void Level::load(std::string filename) {
    loadLevelFile(filename);
    camera.offset = Vector2{GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    camera.target = Vector2{0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 1.f;

	player.spawn(Vec2(500, 500), Vec2(tileSize * 2, tileSize * 3))
		.setMaxSpeeds(10, 10, 8)
		.setForces(0.75, 0.5);
}

void Level::save(std::string filepath) {
    std::string dirpath = filepath.substr(0, filepath.rfind('/'));
    std::filesystem::create_directories(dirpath);
    std::ofstream outf(filepath);
    if(!outf) {
        throw std::runtime_error("Open file error");
    }
    for (int i = 0; i < levelSizeY; i++) {
        for (int j = 0; j < levelSizeX; j++) {
            if (tiles[i][j].getId() != 0) {
                outf << tiles[i][j].getPos().x << " " 
                << tiles[i][j].getPos().y << " " 
                << tiles[i][j].getId() << " " 
                << tiles[i][j].getForm() << " "
                << tiles[i][j].isUp << " " 
                << tiles[i][j].isDown << " " 
                << tiles[i][j].isLeft << " " 
                << tiles[i][j].isRight << " " 
                << tiles[i][j].canClimbLeft << " "
                << tiles[i][j].canClimbRight << std::endl;
            }
        }
    }
    outf.close();
}

bool Level::isTile(Vector2 pos) const {
    return tiles[pos.y / tileSize][pos.x / tileSize].getId() != 0;
}

void setClimb(std::vector<std::vector<Tile>>& tiles, unsigned idY, unsigned idX) {
    if(idY >= 3) {
        tiles[idY][idX].canClimbLeft = (tiles[idY - 1][idX].getId() == 0 && tiles[idY - 2][idX].getId() == 0 && tiles[idY - 3][idX].getId() == 0 && tiles[idY - 3][idX - 1].getId() == 0 && idY > Level::levelOffset + 2);
        tiles[idY][idX].canClimbRight = (tiles[idY - 1][idX].getId() == 0 && tiles[idY - 2][idX].getId() == 0 && tiles[idY - 3][idX].getId() == 0 && tiles[idY - 3][idX + 1].getId() == 0 && idY > Level::levelOffset + 2);
    }
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
    if(isAdded) {
        setClimb(tiles, idY, idX);
    }
    setClimb(tiles, idY + 1, idX);
    setClimb(tiles, idY + 2, idX);
    setClimb(tiles, idY + 3, idX);
    setClimb(tiles, idY + 3, idX - 1);
    setClimb(tiles, idY + 3, idX + 1);
}

void Level::cameraOnBoard() {
    if(camera.target.x - camera.offset.x < tileSize * levelOffset) {
        camera.target.x = tileSize * levelOffset + camera.offset.x;
    }
    if(camera.target.y - camera.offset.y < tileSize * levelOffset) {
        camera.target.y = tileSize * levelOffset + camera.offset.y;
    }
    if(camera.target.x + camera.offset.x > (levelSizeX - levelOffset) * tileSize) {
        camera.target.x = (levelSizeX - levelOffset) * tileSize - camera.offset.x;
    }
    if(camera.target.y + camera.offset.y > (levelSizeY - levelOffset) * tileSize) {
        camera.target.y = (levelSizeY - levelOffset) * tileSize - camera.offset.y;
    }
}

void Level::placeTile(const Tile& tile) {
    Vector2 pos = GetScreenToWorld2D(tile.getPos().toRaylib(), camera);
    unsigned idX = pos.x / tileSize;
    unsigned idY = pos.y / tileSize;
    if(idX < 1 || idY < 1 || idX > levelSizeX - 1 || idY > levelSizeY - 1) {
        return;
    }
    if(isTile(pos)) {
        return;
    }
    if(player.checkCollision(Tile(Vec2(pos.x, pos.y), Vec2(tileSize, tileSize)))) {
        return;
    }
    tiles[idY][idX] = tile;
    tiles[idY][idX].setId(1).spawn(Vec2((int)pos.x - (int)pos.x % tileSize + tileSize / 2, (int)pos.y - (int)pos.y % tileSize + tileSize / 2), tile.getSize());
    setLocalPos(tiles, idY, idX, true);
}

void Level::breakTile(const Tile& tile) {
    Vector2 pos = GetScreenToWorld2D(tile.getPos().toRaylib(), camera);
    unsigned idX = pos.x / tileSize;
    unsigned idY = pos.y / tileSize;
    if(idX < 1 || idY < 1 || idX > levelSizeX - 1 || idY > levelSizeY - 1) {
        return;
    }
    if(!isTile(pos)) {
        return;
    }
    tiles[idY][idX].setId(0).setForm(0);
    setLocalPos(tiles, idY, idX, false);
}

void Level::calcCords() {
    startRenderY = (camera.target.y - camera.offset.y / camera.zoom) / tileSize - 5;
    endRenderY = (camera.target.y + camera.offset.y / camera.zoom) / tileSize + 5;
    startRenderX = (camera.target.x - camera.offset.x / camera.zoom) / tileSize - 5;
    endRenderX = (camera.target.x + camera.offset.x / camera.zoom) / tileSize + 5;
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
}

void Level::render() {
    BeginMode2D(camera);
    
    this->calcCords();
    player.render();
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
    cameraOnBoard();
    player.onBoard();
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