#pragma once
#include <string>
#include <vector>
#include "tile.h"

class Player;

static const unsigned tileSize = 64;
static const unsigned levelSizeX = 2000;
static const unsigned levelSizeY = 1000;

class Level {
    public:
        Level();
        void setCamera(const Camera2D* camera);
        void loadLevel(std::string filename);
        void saveLevel(std::string filename);

        bool isTile(const Tile& tile) const;
        void placeTile(const Tile& tile);
        void breakTile(const Tile& tile);

        void calcCords();

        void render();

        void checkCollision(Player& pl);

        ~Level();

    private:
        unsigned long startRenderX, endRenderX, startRenderY, endRenderY;
        const Camera2D* camera;
        std::vector<std::vector<Tile>> tiles;
};