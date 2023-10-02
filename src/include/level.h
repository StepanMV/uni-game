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

        void load(std::string filename);
        void save(std::string filename);

        bool isTile(Vector2 localPos) const;
        void placeTile(const Vec2 tilePos);
        void breakTile(const Vec2 tilePos);

        void calcCords();

        void render();
        void update();

        void checkCollision();

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