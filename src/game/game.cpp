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
	ToggleFullscreen();
}

void Game::loadLevel() {
	Renderer::loadTextures("resources/textures");
	Renderer::loadTextures("resources/sprites");
	level.load("../../saves/level.txt");
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
	
	level.update();
	
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Tile tile(Vec2(GetMouseX(), GetMouseY()), Vec2(64, 64));
        level.placeTile(tile);
    }
    if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Tile tile(Vec2(GetMouseX(), GetMouseY()), Vec2(64, 64));
        level.breakTile(tile);
    }
}

void Game::draw() {
	BeginDrawing();

		ClearBackground(BLACK);

		level.render();
        DrawFPS(10, 10);
        
	EndDrawing();
}