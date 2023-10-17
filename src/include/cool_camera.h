#pragma once

#include <memory>
#include "raylib.h"
#include "vec2.h"

class CoolCamera{
public:
    static std::shared_ptr<CoolCamera> init(Vec2* target = nullptr);

    void setTarget(Vec2* target);
    void update();

    Vector4 getRenderBounds() const;
    Camera2D getCamera() const;


private:
    std::shared_ptr<Camera2D> camera;
    Vec2* position;
};