#include "cool_camera.h"
#include "raylib.h"
#include "game.h"

std::shared_ptr<CoolCamera> CoolCamera::init() {
    auto coolCamera = std::make_shared<CoolCamera>();
    coolCamera->camera = std::make_shared<Camera2D>();
    coolCamera->camera->offset = Vector2{GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    coolCamera->camera->rotation = 0.0f;
    coolCamera->camera->zoom = 1.0f * Game::settings->readDouble("Runtime", "screenCoefW", 1);
    return coolCamera;
}

void CoolCamera::update(Vec2 position) {
    auto tileSize = Level::tileSize;
    auto width = Level::width;
    auto height = Level::height;
    auto borderOffset = Level::borderOffset;
    camera->target = position.toRaylib();

    // Keep camera inside level bounds
    if(camera->target.x - camera->offset.x < tileSize * borderOffset) {
        camera->target.x = tileSize * borderOffset + camera->offset.x;
    }
    if(camera->target.y - camera->offset.y < tileSize * borderOffset) {
        camera->target.y = tileSize * borderOffset + camera->offset.y;
    }
    if(camera->target.x + camera->offset.x > (width - borderOffset) * tileSize) {
        camera->target.x = (width - borderOffset) * tileSize - camera->offset.x;
    }
    if(camera->target.y + camera->offset.y > (height - borderOffset) * tileSize) {
        camera->target.y = (height - borderOffset) * tileSize - camera->offset.y;
    }
}

void CoolCamera::setZoom(float zoom) {
    if (zoom < 0.25f) zoom = 0.25f;
    if (zoom > 5.0f) zoom = 5.0f;
    camera->zoom = zoom;
}

Camera2D CoolCamera::getCamera() const {
    return *camera;
}

Vector4 CoolCamera::getRenderBounds() const {
    auto target = camera->target;
    auto offset = camera->offset;
    auto zoom = camera->zoom;
    int startRenderY = (target.y - offset.y / zoom) / Level::tileSize - 5;
    int endRenderY = (target.y + offset.y / zoom) / Level::tileSize + 5;
    int startRenderX = (target.x - offset.x / zoom) / Level::tileSize - 5;
    int endRenderX = (target.x + offset.x / zoom) / Level::tileSize + 5;
    if (startRenderX < 0) startRenderX = 0;
    if (startRenderY < 0) startRenderY = 0;
    if (endRenderX > Level::width) endRenderX = Level::width;
    if (endRenderY > Level::height) endRenderY = Level::height;
    return Vector4{(float) startRenderX, (float) endRenderX, (float) startRenderY, (float) endRenderY};
}
