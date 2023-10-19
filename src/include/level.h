#pragma once

#include <string>
#include <vector>
#include "ui.h"
#include "cool_camera.h"

class Tile;
class Projectile;
class Player;
class Object;
class Enemy;


class Level {
public:
    Level();
    ~Level();

    void loadGame(std::string filepath, unsigned int levelID);
    void loadEditor(std::string filepath, unsigned int levelID);
    void save();

    bool isTile(Vector2 localPos) const;
    void placeTile(const Vec2 tilePos, int id);
    void breakTile(const Vec2 tilePos);

    void render();
    void update();

    bool isLoaded() const;
    void unload();

    static const unsigned tileSize;
    static const unsigned width;
    static const unsigned height;
    static const unsigned borderOffset;
    static std::shared_ptr<CoolCamera> camera;

private:
    void loadFile(std::string filepath);
    void updateEditor();

    void setLocalPos(unsigned& idY, unsigned& idX, bool isAdded);
    void setClimb(unsigned idY, unsigned idX);

    std::shared_ptr<Player> player;

    bool loaded = false;
    bool editor = false;
    int placedBlockId = 1;
    unsigned int id = 0;

    std::string filepath;
};