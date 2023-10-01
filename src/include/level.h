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
        void load(std::string filepath);
        void save(std::string filepath);

        bool isTile(Vector2 localPos) const;
        void placeTile(const Tile& tile);
        void breakTile(const Tile& tile);

        void calcCords();

        void render();
        void update();

        void checkCollision();

        void cameraOnBoard();

        ~Level();

        static const unsigned tileSize;
        static const unsigned levelSizeX;
        static const unsigned levelSizeY;
        static const unsigned levelOffset;

    private:
        Player player;
        Camera2D camera;

        std::string filename;
        unsigned long startRenderX, endRenderX, startRenderY, endRenderY;
        std::vector<std::vector<Tile>> tiles;
        
};