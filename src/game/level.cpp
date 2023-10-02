#include "level.h"
#include "player.h"
#include <fstream>

const unsigned Level::tileSize = 64;
const unsigned Level::levelSizeX = 2000;
const unsigned Level::levelSizeY = 1000;

//posX, posY, sizeX, sizeY, id, form, isUp, isDown, isLeft, isRight
Level::Level() {
    tiles.resize(levelSizeY);
    for(int i = 0; i < levelSizeY; i++) {
        tiles[i].resize(levelSizeX);
        for(int j = 0; j < levelSizeX; j++) {
            tiles[i][j] = Tile();
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
            tiles[posY / tileSize][posX / tileSize] = TileBuilder::spawn(Vec2(posX, posY), Vec2(sizeX, sizeY))
                                                                    .setID(id)
                                                                    .setForm(form)
                                                                    .setNeighbors(isUp, isDown, isLeft, isRight)
                                                                    .build();
        }
        inf.close();
    }
    // ==============================
    
    camera.offset = Vector2{GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    camera.target = Vector2{0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 0.25f;

    player = std::move(PlayerBuilder::spawn(Vec2(500 * tileSize, 500 * tileSize), Vec2(tileSize * 2, tileSize * 3))
        .setMaxSpeeds(50, 30, 20)
        .setForces(0.5, 0.75)
        .setHeadTexture("resources/textures/Armor_Head_3.png")
        .setBodyTexture("resources/textures/Armor_3.png")
        .setLegsTexture("resources/textures/Armor_Legs_3.png")
        .build());
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

void Level::placeTile(const Vec2 tilePos) {
    // Возможно проще будет преобразовать pos в позицию блока, а не в координаты
    // Условие длинновато
    if(tilePos.x < tileSize || tilePos.y < tileSize || tilePos.x > (levelSizeX - 1) * tileSize || tilePos.y > (levelSizeY - 1) * tileSize) {
        return;
    }
    if(isTile(tilePos.toRaylib())) {
        return;
    }
    unsigned idX = tilePos.x / tileSize;
    unsigned idY = tilePos.y / tileSize;
    tiles[idY][idX] = TileBuilder::spawn({(idX + 0.5f) * tileSize, (idY + 0.5f) * tileSize}, {tileSize, tileSize}).setID(1).build();
    setLocalPos(tiles, idY, idX, true);
}

void Level::breakTile(const Vec2 tilePos) {
    // Тоже
    if(tilePos.x < tileSize || tilePos.y < tileSize || tilePos.x > (levelSizeX - 1) * tileSize || tilePos.y > (levelSizeY - 1) * tileSize) {
        return;
    }
    if(!isTile(tilePos.toRaylib())) {
        return;
    }
    unsigned idX = tilePos.x / tileSize;
    unsigned idY = tilePos.y / tileSize;
    setLocalPos(tiles, idY, idX, false);
    tiles[idY][idX] = Tile();
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
    DrawText(std::to_string(player.getPos().x / tileSize).c_str(), 10, 40, 20, RED);
    DrawText(std::to_string(player.getPos().y / tileSize).c_str(), 10, 70, 20, RED);
}

void Level::update() {
	player.move();
    camera.target = player.getPos().toRaylib();
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