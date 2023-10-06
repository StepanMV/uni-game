#include "tile.h"

unsigned Tile::getId() const {
    return id;
}

unsigned Tile::getForm() const {
    return form;
}

void Tile::updateState() {
    std::string state = "";
    state += isUp ? "u" : "0";
    state += isDown ? "d" : "0";
    state += isLeft ? "l" : "0";
    state += isRight ? "r" : "0";
    renderer.setState(state);
}

void Tile::update() { }

void Tile::render() {
    renderer.render();
}

TileBuilder TileBuilder::spawn(Vec2 pos, Vec2 size) {
    TileBuilder builder;
    builder.tile.pos = pos;
    builder.tile.size = size;
    return builder;
}

TileBuilder &TileBuilder::setNeighbors(bool up, bool down, bool left, bool right) {
    tile.isUp = up;
    tile.isDown = down;
    tile.isLeft = left;
    tile.isRight = right;
    return *this;
}

TileBuilder &TileBuilder::setClimb(bool left, bool right) {
    tile.canClimbLeft = left;
    tile.canClimbRight = right;
    return *this;
}

TileBuilder &TileBuilder::setID(unsigned id) {
    tile.id = id;
    return *this;
}

TileBuilder &TileBuilder::setForm(unsigned form) {
    tile.form = form;
    return *this;
}

Tile TileBuilder::build() {
    if (tile.id == 0) return Tile();

    tile.renderer.loadTexture("texture", "resources/textures/Tiles_" + std::to_string(tile.id) + ".png");

    Vec2 textureSize = tile.renderer.getTextureSize("texture");

    tile.renderer.addToState("0000", "main", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "texture", textureSize)
        .spriteSheet({16, 15}, {GetRandomValue(9, 11), 3}).build());
    tile.renderer.addToState("000r", "main", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "texture", textureSize)
        .spriteSheet({16, 15}, {9, GetRandomValue(0, 2)}).build());
    tile.renderer.addToState("00l0", "main", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "texture", textureSize)
        .spriteSheet({16, 15}, {12, GetRandomValue(0, 2)}).build());
    tile.renderer.addToState("00lr", "main", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "texture", textureSize)
        .spriteSheet({16, 15}, {GetRandomValue(6, 8), 4}).build());
    tile.renderer.addToState("0d00", "main", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "texture", textureSize)
        .spriteSheet({16, 15}, {GetRandomValue(6, 8), 0}).build());
    tile.renderer.addToState("0d0r", "main", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "texture", textureSize)
        .spriteSheet({16, 15}, {0, 3}).build());
    tile.renderer.addToState("0dl0", "main", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "texture", textureSize)
        .spriteSheet({16, 15}, {1, 3}).build());
    tile.renderer.addToState("0dlr", "main", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "texture", textureSize)
        .spriteSheet({16, 15}, {GetRandomValue(1, 3), 0}).build());
    tile.renderer.addToState("u000", "main", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "texture", textureSize)
        .spriteSheet({16, 15}, {GetRandomValue(6, 8), 3}).build());
    tile.renderer.addToState("u00r", "main", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "texture", textureSize)
        .spriteSheet({16, 15}, {0, 4}).build());
    tile.renderer.addToState("u0l0", "main", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "texture", textureSize)
        .spriteSheet({16, 15}, {1, 4}).build());
    tile.renderer.addToState("u0lr", "main", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "texture", textureSize)
        .spriteSheet({16, 15}, {GetRandomValue(1, 3), 2}).build());
    tile.renderer.addToState("ud00", "main", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "texture", textureSize)
        .spriteSheet({16, 15}, {5, GetRandomValue(0, 2)}).build());
    tile.renderer.addToState("ud0r", "main", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "texture", textureSize)
        .spriteSheet({16, 15}, {0, GetRandomValue(0, 2)}).build());
    tile.renderer.addToState("udl0", "main", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "texture", textureSize)
        .spriteSheet({16, 15}, {4, GetRandomValue(0, 2)}).build());
    tile.renderer.addToState("udlr", "main", TextureDataBuilder::init(TextureType::SPRITE_SHEET, "texture", textureSize)
        .spriteSheet({16, 15}, {GetRandomValue(1, 3), 1}).build());
    tile.updateState();
    return tile;
}
