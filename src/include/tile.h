#pragma once
#include "object.h"
#include "vec2.h"

class Tile: public Object {
public:

    unsigned getId() const;
    unsigned getForm() const;

    virtual void update() override;
    virtual void render() override;

    bool isUp = false, isDown = false, isLeft = false, isRight = false;
private:
    friend class TileBuilder;

    unsigned id = 0;
    unsigned form = 0;
};

class TileBuilder {
public:
    static TileBuilder spawn(Vec2 pos, Vec2 size);
    TileBuilder& setNeighbors(bool up, bool down, bool left, bool right);
    TileBuilder& setID(unsigned id);
    TileBuilder& setForm(unsigned form);

    Tile build() const;

private:
    Vec2 pos, size;
    bool up = false, down = false, left = false, right = false;
    unsigned id = 0, form = 0;
};