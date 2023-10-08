#pragma once

#include "renderer.h"
#include "timer.h"
#include <vector>
#include <map>

enum BackgroundType {
    ANIMATED,
    LAYERED
};

struct BackgroundData {
    Color color;
    BackgroundType type;
    int spritesAmount;
};

class Background {
public:
    Background() = default;
    Background(int id);

    static std::shared_ptr<Background> create(int id, float spf = 1);

    void setSpeed(Vec2 speedMultiplier);

    Color getColor() const;

    virtual void update() = 0;

    static const std::map<int, BackgroundData> bgData;

protected:
    Vec2 screenCenter = {(float) (GetScreenWidth() / 2), (float) (GetScreenHeight() / 2)};
    int id = 0;
    Color color = WHITE;
    Vec2 speedMultiplier = {1, 0};
    CoolRenderer renderer;
    static const std::vector<float> speeds;
    static const std::vector<int> destYoffsets;
};

class AnimatedBackground : public Background {
public:
    AnimatedBackground(int id, float spf = 1);

    void setAnimSpeed(int spf);

    void update() override;

protected:
    void changeFrame();

    std::shared_ptr<Timer> timer;
    Vec2 offset;
    int currentFrame;
};

class LayeredBackground : public Background {
public:
    LayeredBackground(int id);

    void update() override;

protected:
    std::vector<Vec2> offsets;
};