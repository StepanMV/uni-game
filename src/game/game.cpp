#include "Game.h"

#include "raylib.h"
#include "player.h"
#include "tile.h"

#include <vector>
#include <iostream>


Game::Game(int width, int height, int fps, std::string title): player(Vec2(1000, 1000), Vec2(180, 240)), tile(Vec2(700, 500), Vec2(128, 128))
{	

	SetTargetFPS(fps);
	InitWindow(width, height, title.c_str());
}

void Game::loadLevel() {
	background = LoadTexture("resources/images/background.png");
    camera.offset = Vector2{GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    camera.target = Vector2{0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 0.5f;
	player.spawn()
		.setMaxSpeeds(50, 50, 20)
		.setForces(1, 0.5);
	tile.spawn();
}

Game::~Game() noexcept
{
	CloseWindow();
}

bool Game::shouldClose() const {
	return WindowShouldClose();
}

void Game::tick() {
	this->update();
	this->draw();
	
}

void Game::update() {
	player.move();
	camera.target = player.getPos().toRaylib();
	player.update();
	this->checkCollisions();
}

void Game::draw() {
	BeginDrawing();

		ClearBackground(BLACK);

        BeginMode2D(camera);

			DrawTextureEx(background, Vector2{-background.width / 2.0f, -background.height / 2.0f}, 0.0f, 1.0f, WHITE);
			player.render();

			tile.render();

        EndMode2D();

        DrawFPS(10, 10);
        
	EndDrawing();
}

void Game::checkCollisions() {
	if (player.checkCollision(tile)) {
		player.onCollision(&tile);
	}
}