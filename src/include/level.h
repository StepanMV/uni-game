#pragma once

#include <string>
#include <vector>
// #include "tile.h"
// #include "projectile.h"
// #include "player.h"
#include "ui.h"

class Tile;
class Projectile;
class Player;
class Object;


class Level {
public:
    Level();
    ~Level();

    void linkUI(std::shared_ptr<UI> ui, std::shared_ptr<Background> bg);
    void loadGame(std::string filepath);
    void loadEditor(std::string filepath);
    void save();

    bool isTile(Vector2 localPos) const;
    void placeTile(const Vec2 tilePos, int id);
    void breakTile(const Vec2 tilePos);

    void calcCords();

    void render();
    void update();

    void checkCollision();

    void cameraOnBoard();

    bool isLoaded() const;

    static const unsigned tileSize;
    static const unsigned levelSizeX;
    static const unsigned levelSizeY;
    static const unsigned levelOffset;

    static void addObject(std::shared_ptr<Object> object);

    void checkObjExisting();

private:
    void loadFile(std::string filepath);
    void updateEditor();

    void setLocalPos(unsigned& idY, unsigned& idX, bool isAdded);
    void setClimb(unsigned idY, unsigned idX);

    static std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::vector<std::shared_ptr<Tile>>> tiles;
    std::shared_ptr<Player> player;

    bool loaded = false;
    bool editor = false;
    int placedBlockId = 1;

    Camera2D camera;
    std::shared_ptr<UI> ui;
    std::shared_ptr<Background> background;

    std::string filepath;
    unsigned long startRenderX, endRenderX, startRenderY, endRenderY;
};