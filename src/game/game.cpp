#include "Game.h"

#include "raylib.h"
#include "player.h"
#include "tile.h"
#include "renderer.h"

#include <vector>
#include <iostream>


Game::Game(int width, int height, int fps, std::string title)
{	
	SetTargetFPS(fps);
	InitWindow(width, height, title.c_str());
}

void Game::loadLevel() {
	Renderer::loadTextures("resources/textures");
	background = LoadTexture("resources/textures/Background_53.png");
    camera.offset = Vector2{GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    camera.target = Vector2{0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 0.5f;
	player.spawn(Vec2(0, -50), Vec2(120, 180))
		.setMaxSpeeds(40, 10, 20)
		.setForces(0.75, 0.5);
	tile.spawn(Vec2(0, 0), Vec2(128, 128));
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
	player.update();
	camera.target = player.getPos().toRaylib();
	this->checkCollisions();
	player.move();
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