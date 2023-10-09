#pragma once
#include "object.h"
#include "vec2.h"

class Tile: public Object {
public:
    Tile();
    Tile(const Tile& other);
    Tile& operator=(const Tile& other);

    unsigned getId() const;
    unsigned getForm() const;

    void updateState();

    virtual bool isAlive() const override;

    virtual void update() override;
    virtual void render() override;

    bool isUp = false, isDown = false, isLeft = false, isRight = false;
    bool canClimbLeft = true, canClimbRight = true;
    bool isPlatform = false;
private:
    friend class TileBuilder;

    unsigned id = 0;
    unsigned form = 0;
};

class TileBuilder {
public:
    static TileBuilder spawn(Vec2 pos, Vec2 size);
    TileBuilder& setNeighbors(bool up, bool down, bool left, bool right);
    TileBuilder& setClimb(bool left, bool right);
    TileBuilder& setID(unsigned id);
    TileBuilder& setForm(unsigned form);

    Tile build();

private:
    Tile tile;
};