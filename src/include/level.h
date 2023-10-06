#pragma once

#include <string>
#include <vector>
#include "tile.h"
#include "player.h"


class Level {
public:
    Level();

    Level(const Level& other) = delete;
    Level& operator=(const Level& other) = delete;

    void loadLevelFile(std::string filepath);
    void load(std::string filepath, bool editor = false);
    void save(std::string filepath);

    bool isTile(Vector2 localPos) const;
    void placeTile(const Vec2 tilePos, int id);
    void breakTile(const Vec2 tilePos);

    void calcCords();

    void render();
    void update();

    void checkCollision();

    void cameraOnBoard();

    bool isLoaded() const;

    ~Level();

    static const unsigned tileSize;
    static const unsigned levelSizeX;
    static const unsigned levelSizeY;
    static const unsigned levelOffset;

private:
    bool loaded = false, editor = false;
    int placedBlockId = 1;
    int j = 1;

    Player player;
    Camera2D camera;

    std::string filename;
    unsigned long startRenderX, endRenderX, startRenderY, endRenderY;
    std::vector<std::vector<Tile>> tiles;
};