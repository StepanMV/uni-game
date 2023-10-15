#pragma once
#include "object.h"
#include "vec2.h"

class Tile: public Object {
public:
    template<class... Args>
    static std::shared_ptr<Tile> createTile(Args&&... args) {
        return std::shared_ptr<Tile>(new Tile(args...));
    }

    unsigned getId() const;
    unsigned getForm() const;

    template<class... Args>
    static void createObj(Args&&... args) = delete;

    virtual void onCollision(std::shared_ptr<Tile> other) override;
    virtual void onCollision(std::shared_ptr<Enemy> other) override;
    virtual void onCollision(std::shared_ptr<Player> other) override;
    virtual void onCollision(std::shared_ptr<Projectile> other) override;
    virtual bool isCollideable() const override;
    virtual void breakObject() override;

    void updateState();

    virtual bool isAlive() const override;
    virtual void update() override;
    virtual void render() override;

    bool isUp = false, isDown = false, isLeft = false, isRight = false;
    bool canClimbLeft = true, canClimbRight = true;
    bool isPlatform = false;
private:

    Tile();
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