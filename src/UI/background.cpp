#include "background.h"
#include "level.h"
#include <memory>
#include <stdexcept>
#include "game.h"
#include <iostream>

const std::map<int, BackgroundData> Background::bgData = {
    {1, {BLUE, LAYERED, 4}},
    {2, {BLUE, ANIMATED, 4}}
};

const std::vector<float> Background::speeds = {0.3f, 0.45f, 0.6f, 0.75f};
const std::vector<int> Background::destYoffsets = {500, 400, 300, 200};

Background::Background(int id) :
    id(id),
    color(bgData.at(id).color),
    renderer(std::make_shared<MyTransform>(screenCenter, Vec2(0, 0), 0))
{ }

std::shared_ptr<Background> Background::create(int id, float spf) {
    switch (bgData.at(id).type) {
        case ANIMATED:
            return std::make_shared<AnimatedBackground>(id, spf);
        case LAYERED:
            return std::make_shared<LayeredBackground>(id);
        default:
            throw std::runtime_error("Wrong background type");
    }
}

void Background::setSpeed(Vec2 speedMultiplier)
{
    this->speedMultiplier = speedMultiplier;
}

Color Background::getColor() const {
    return color;
}

AnimatedBackground::AnimatedBackground(int id, float spf) : Background(id) {
    if (bgData.at(id).type != ANIMATED) throw std::runtime_error("Wrong background type");
    timer = Timer::getInstance(spf, false, [this]() { changeFrame(); });
    for (int i = 0; i < bgData.at(id).spritesAmount; i++) {
        Vec2 textureSize = renderer.loadTexture("bg " + std::to_string(i), "resources/textures/Background_" + std::to_string(id) + "_" + std::to_string(i) + ".png");
        startOffset = {
            textureSize.x / 3 + GetRandomValue(-300, 300) * (float) Game::settings->readDouble("Runtime", "screenCoefW", 1),
            screenCenter.y - destYoffsets[1] * (float) Game::settings->readDouble("Runtime", "screenCoefH", 1)};
        renderer.addToState("main " + std::to_string(i), "bg " + std::to_string(i),
            TextureDataBuilder::init(TextureType::CROPPED_TEXTURE, "bg " + std::to_string(i), textureSize)
            .croppedTexture({startOffset.x, 0})
            .setDestOffset({0, startOffset.y})
            .setExtra(0, 0, 3 * screenCenter.x * 2 / textureSize.x)
            .build()
        );
    }
}

void AnimatedBackground::setAnimSpeed(int spf) {
    timer = Timer::getInstance(spf, false, [this]() { changeFrame(); });
}

void AnimatedBackground::update() {
    offset.x += speeds[1] * speedMultiplier.x * (float) Game::settings->readDouble("Runtime", "screenCoefW", 1);
    offset.y -= speeds[1] * speedMultiplier.y * (float) Game::settings->readDouble("Runtime", "screenCoefH", 1);
    renderer.setOffset({startOffset.x + offset.x, 0});
    renderer.setDestOffset({0, startOffset.y + offset.y});
    renderer.render();
}

void AnimatedBackground::changePos(Vec2 pos) {
    offset.x = 0.1 * speeds[1] * pos.x * (float) Game::settings->readDouble("Runtime", "screenCoefW", 1);
    offset.y = 0.1 * speeds[1] * pos.y * (float) Game::settings->readDouble("Runtime", "screenCoefH", 1);
}

void AnimatedBackground::changeFrame() {
    currentFrame++;
    currentFrame %= bgData.at(id).spritesAmount;
    renderer.setState("main " + std::to_string(currentFrame));
    timer->reset();
}

LayeredBackground::LayeredBackground(int id) : Background(id)
{
    if (bgData.at(id).type != LAYERED) throw std::runtime_error("Wrong background type");
    for (int i = 0; i < bgData.at(id).spritesAmount; i++) {
        Vec2 textureSize = renderer.loadTexture("bg " + std::to_string(i), "resources/textures/Background_" + std::to_string(id) + "_" + std::to_string(i) + ".png");
        Vec2 startOffset = {
            textureSize.x / 3 + GetRandomValue(-300, 300) * (float) Game::settings->readDouble("Runtime", "screenCoefW", 1),
            screenCenter.y - destYoffsets[i] * (float) Game::settings->readDouble("Runtime", "screenCoefH", 1)};
        startOffsets.push_back(startOffset);
        offsets.push_back({0, 0});
        renderer.addToState("main", "bg " + std::to_string(i),
            TextureDataBuilder::init(TextureType::CROPPED_TEXTURE, "bg " + std::to_string(i), textureSize)
            .croppedTexture({startOffset.x, 0})
            .setDestOffset({0, startOffset.y})
            .setExtra(0, 0, 3 * screenCenter.x * 2 / textureSize.x)
            .build()
        );
    }
    renderer.setState("main");
}

void LayeredBackground::update() {
    for (int i = 0; i < bgData.at(id).spritesAmount; i++) {
        offsets[i].x += speeds[i] * speedMultiplier.x * (float) Game::settings->readDouble("Runtime", "screenCoefW", 1);
        offsets[i].y -= speeds[i] * speedMultiplier.y * (float) Game::settings->readDouble("Runtime", "screenCoefH", 1);
        renderer.setOffset({startOffsets[i].x + offsets[i].x, 0}, "bg " + std::to_string(i));
        renderer.setDestOffset({0, startOffsets[i].y + offsets[i].y}, "bg " + std::to_string(i));
    }
    renderer.render();
}

void LayeredBackground::changePos(Vec2 pos) {
    for (int i = 0; i < bgData.at(id).spritesAmount; i++) {
        offsets[i].x = -0.1 * speeds[i] * pos.x * (float) Game::settings->readDouble("Runtime", "screenCoefW", 1);
        offsets[i].y = 0.1 * speeds[i] * pos.y * (float) Game::settings->readDouble("Runtime", "screenCoefH", 1);
    }
}
