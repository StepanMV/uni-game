#include "Game.h"

#include "raylib.h"
#include "player.h"
#include "tile.h"

#include <vector>
#include <iostream>


Game::Game(int width, int height, int fps, std::string title): player(Vec2(1000, 1000), Vec2(128, 217)), tile(Vec2(700, 500), Vec2(64, 64)), tile1(Vec2(636, 536), Vec2(64, 64))
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
		.setMaxSpeeds(50, 30, 20)
		.setForces(0.75, 0.5);
	tile.spawn();
	tile1.spawn();
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
			tile1.render();

        EndMode2D();

        DrawFPS(10, 10);
        
	EndDrawing();
}

void Game::checkCollisions() {
		if (player.checkCollision(tile1)) {
		player.onCollision(&tile1);
	}
	if (player.checkCollision(tile)) {
		player.onCollision(&tile);
	}

}