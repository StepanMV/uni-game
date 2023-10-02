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
            tiles[i][j] = Tile();
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
            tiles[posY / tileSize][posX / tileSize] = std::move(TileBuilder::spawn({(float) posX, (float) posY}, {tileSize, tileSize})
                .setID(id)
                .setForm(form)
                .setNeighbors(isUp, isDown, isLeft, isRight)
                .setClimb(canClimbLeft, canClimbRight)
                .build());
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

    player = std::move(PlayerBuilder::spawn(Vec2(500 * tileSize, 500 * tileSize), Vec2(tileSize * 2, tileSize * 3))
        .setMaxSpeeds(10, 10, 8)
        .setForces(0.5, 0.75)
        .setHeadTexture("resources/textures/Armor_Head_3.png")
        .setBodyTexture("resources/textures/Armor_3.png")
        .setLegsTexture("resources/textures/Armor_Legs_3.png")
        .build());
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

void Level::placeTile(const Vec2 tilePos) {
    unsigned idX = tilePos.x / tileSize;
    unsigned idY = tilePos.y / tileSize;

    if(idX < 1 || idY < 1 || idX > levelSizeX - 1 || idY > levelSizeY - 1) return;
    if(isTile(tilePos.toRaylib())) return;

    Tile tile = std::move(TileBuilder::spawn({(idX + 0.5f) * tileSize, (idY + 0.5f) * tileSize}, {tileSize, tileSize}).setID(1).build());

    if(player.checkCollision(tile)) return;

    tiles[idY][idX] = std::move(tile);
    setLocalPos(tiles, idY, idX, true);
}

void Level::breakTile(const Vec2 tilePos) {
    unsigned idX = tilePos.x / tileSize;
    unsigned idY = tilePos.y / tileSize;

    if(idX < 1 || idY < 1 || idX > levelSizeX - 1 || idY > levelSizeY - 1) return;
    if(!isTile(tilePos.toRaylib())) return;

    setLocalPos(tiles, idY, idX, false);
    tiles[idY][idX] = std::move(Tile());
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
    DrawText(std::to_string(player.getPos().x / tileSize).c_str(), 10, 40, 20, RED);
    DrawText(std::to_string(player.getPos().y / tileSize).c_str(), 10, 70, 20, RED);
}

void Level::update() {
	player.move();
    camera.target = player.getPos().toRaylib();
    cameraOnBoard();
    player.onBoard();
    player.update();
    this->checkCollision();

    Vector2 mousePos = GetScreenToWorld2D({(float) GetMouseX(), (float) GetMouseY()}, camera);
    Vec2 mp = {mousePos.x, mousePos.y};
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        placeTile(mp);
    }
    if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        breakTile(mp);
    }
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