#include "Game.h"

#include "raylib.h"
#include "player.h"
#include "tile.h"
#include "renderer.h"

#include <vector>
#include <iostream>


Game::Game(int width, int height, int fps, std::string title)
{	
	level.setCamera(&camera);
	SetTargetFPS(fps);
	InitWindow(width, height, title.c_str());
}

void Game::loadLevel() {
	Renderer::loadTextures("resources/textures");
	Renderer::loadTextures("resources/sprites");
	background = LoadTexture("resources/textures/Background_53.png");
    camera.offset = Vector2{GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    camera.target = Vector2{0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 0.5f;
	player.spawn(Vec2(0, -200), Vec2(128, 40))
		.setMaxSpeeds(50, 30, 20)
		.setForces(0.75, 0.5);
	level.loadLevel("../../saves/level.txt");
	player.setLevel(&level);
}

Game::~Game() noexcept
{
	Renderer::unloadTextures();
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
			level.calcCords();
			level.render();
			for (auto& tile : tiles) {
				tile.render();
			}

        EndMode2D();

        DrawFPS(10, 10);
        
	EndDrawing();
}

void Game::checkCollisions() {
	level.checkCollision(player);
}