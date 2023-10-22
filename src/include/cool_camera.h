#pragma once

#include <memory>
#include "raylib.h"
#include "vec2.h"

class CoolCamera {
public:
    static std::shared_ptr<CoolCamera> init();

    void update(Vec2 position);

    Vector4 getRenderBounds() const;
    Camera2D getCamera() const;

private:
    std::shared_ptr<Camera2D> camera;
};