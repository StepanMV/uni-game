#include "Game.h"
#include "raylib.h"

Game::Game(int width, int height, int fps, std::string title)
{
    camera = {0};
    camera.offset = Vector2{width / 2.0f, height / 2.0f};
    camera.target = Vector2{0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

	SetTargetFPS(fps);
	InitWindow(width, height, title.c_str());
}

Game::~Game() noexcept
{
	CloseWindow();
}

bool Game::shouldClose() const {
	return WindowShouldClose();
}

void Game::tick() {
	BeginDrawing();

        BeginMode2D(camera);

	        this->update();
	        this->draw();

        EndMode2D();

        DrawFPS(10, 10);
        
	EndDrawing();
}

void Game::update() {

}

void Game::draw() {
	ClearBackground(BLACK);
}