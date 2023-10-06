#include "Game.h"

#include "raylib.h"
#include "player.h"
#include "tile.h"
#include "renderer.h"
#include "keyboard.h"

#include <vector>
#include <iostream>


Game::Game(int width, int height, int fps, std::string title)
{	
	SetTargetFPS(fps);
	InitWindow(width, height, title.c_str());
	//ToggleFullscreen();
}

Game::~Game() noexcept
{
	Renderer::unloadTextures();
	CloseWindow();
}

bool Game::shouldClose() const {
	return ui.isButtonPressed("exit") || WindowShouldClose();
}

void Game::tick() {
	this->update();
	this->draw();
	
}
/*
continue = GuiButton((Rectangle){ 840, 432, 240, 48 }, "CONTINUE"); 
            editor = GuiButton((Rectangle){ 840, 504, 240, 48 }, "EDITOR"); 
            settings = GuiButton((Rectangle){ 840, 696, 240, 48 }, "SETTINGS"); 
            exit = GuiButton((Rectangle){ 840, 768, 240, 48 }, "EXIT"); 
            GuiDummyRec((Rectangle){ 840, 600, 240, 24 }, "ENEMY SELECTOR");
            if (GuiDropdownBox((Rectangle){ 840, 624, 240, 48 }, "KING SLIME;EYE OF CTHULHU;EATER OF WORLDS", &bossDropdownActive, bossDropdownEditMode))
			*/

void Game::load() {
	Renderer::loadTextures("resources/textures");
	Renderer::loadTextures("resources/sprites");
	Keyboard::init();
	ui = std::move(UIBuilder()
	.addButton("continue", ButtonData{ Rectangle{ 840, 432, 240, 48 }, "CONTINUE" }, [&]() { level.load("saves/level.txt", false); ui = UI(); } )
	.addButton("exit", ButtonData{ Rectangle{ 840, 768, 240, 48 }, "EXIT" })
	.addButton("editor", ButtonData{ Rectangle{ 840, 504, 240, 48 }, "EDITOR" }, [&]() { level.load("saves/level.txt", true); ui = UI(); } )
	.addButton("settings", ButtonData{ Rectangle{ 840, 696, 240, 48 }, "SETTINGS" }, [&]() { std::cout << "SETTINGS" << std::endl; } )
	.addDummyRect("enemySelector", DummyRectData{ Rectangle{ 840, 600, 240, 24 }, "ENEMY SELECTOR" })
	.addDropdown("bossDropdown", DropdownData{ Rectangle{ 840, 624, 240, 48 }, "KING SLIME;EYE OF CTHULHU;EATER OF WORLDS" })
	.build());
}

void Game::update() {
	Keyboard::update();
	Timer::updateAll();

	ui.update();
	if(level.isLoaded()) level.update();

	if (Keyboard::isDoublePressed(KEY_F)) std::cout << "F" << std::endl;
}

void Game::draw() {
	BeginDrawing();

		ClearBackground(BLACK);

		if(level.isLoaded()) level.render();
        DrawFPS(10, 10);
        
	EndDrawing();
}