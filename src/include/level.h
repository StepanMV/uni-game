#pragma once

#include <string>
#include <vector>
#include "tile.h"
#include "player.h"


class Level {
    public:
        Level();

        // Не думаю что уровень придётся копировать, поэтому наверное стоит удалить, чтобы не сделать этого случайно
        Level(const Level& other) = delete;
        Level& operator=(const Level& other) = delete;

        void loadLevel(std::string filename);
        void saveLevel(std::string filename);

        bool isTile(Vector2 localPos) const;
        void placeTile(const Tile& tile);
        void breakTile(const Tile& tile);

        void calcCords();

        void render();
        void update();

        void checkCollision(Player& pl);

        ~Level();

    private:
        // Переместил этих челиков сюда, потому что они не нужны вне класса
        Player player;
        Camera2D camera;
        // ========================

        std::string filename;
        unsigned long startRenderX, endRenderX, startRenderY, endRenderY;
        std::vector<std::vector<Tile>> tiles;
        
        static const unsigned tileSize;
        static const unsigned levelSizeX;
        static const unsigned levelSizeY;
};