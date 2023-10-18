#pragma once
#include "object.h"
#include "vec2.h"

class Tile: public Object {
public:
    unsigned getForm() const;

    virtual void onCollision(std::shared_ptr<Tile> other) override;
    virtual void onCollision(std::shared_ptr<Enemy> other) override;
    virtual void onCollision(std::shared_ptr<Player> other) override;
    virtual void onCollision(std::shared_ptr<Projectile> other) override;
    virtual bool isCollideable() const override;

    void updateState();

    virtual void update() override {};
    virtual void render() override {};

    bool isUp = false, isDown = false, isLeft = false, isRight = false;
    bool canClimbLeft = true, canClimbRight = true;
    bool isPlatform = false;
private:

    Tile();
    friend class TileBuilder;
    unsigned form = 0;
};

class TileBuilder {
public:
    static TileBuilder spawn(unsigned id, Vec2 pos, Vec2 size);
    TileBuilder& setNeighbors(bool up, bool down, bool left, bool right);
    TileBuilder& setClimb(bool left, bool right);
    TileBuilder& setForm(unsigned form);

    std::shared_ptr<Tile> build();

private:
    std::shared_ptr<Tile> tile;
};