#include "Game.h"

#include "raylib.h"
#include "player.h"
#include "tile.h"
#include "renderer.h"
#include "keyboard.h"

#include <vector>
#include <iostream>

Game::Game(int width, int height, int fps, std::string title) {	
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
	return ui->isButtonPressed("exit");
}

void Game::tick() {
	this->update();
	this->draw();
	
}

void Game::load() {
	Renderer::loadTextures("resources/textures");
	Keyboard::init();
	createUIS();
	ui = uis.at("startMenu");
	background = Background::create(2, 0.25);
}

void Game::createUIS() {
	const_cast<std::map<std::string, std::shared_ptr<UI>>&>(uis) = {
		{"startMenu", UIBuilder()
			.addButton("start", ButtonData{ Rectangle{ 840, 432, 240, 48 }, "START" })
			.addButton("exit", ButtonData{ Rectangle{ 840, 768, 240, 48 }, "EXIT" })
			.addButton("editor", ButtonData{ Rectangle{ 840, 504, 240, 48 }, "EDITOR" })
			.addButton("settings", ButtonData{ Rectangle{ 840, 696, 240, 48 }, "SETTINGS" })
			.addDummyRect("enemySelector", DummyRectData{ Rectangle{ 840, 600, 240, 24 }, "ENEMY SELECTOR" })
			.addDropdown("bossDropdown", DropdownData{ Rectangle{ 840, 624, 240, 48 }, "KING SLIME;EYE OF CTHULHU;EATER OF WORLDS" })
			.build()
		},
		{"pauseMenu", UIBuilder()
			.addButton("continue", ButtonData{ Rectangle{ 792, 312, 336, 48 }, "CONTINUE" })
			.addButton("exitMenu", ButtonData{ Rectangle{ 792, 408, 336, 48 }, "EXIT TO MENU" })
			.addButton("exit", ButtonData{ Rectangle{ 792, 504, 336, 48 }, "EXIT" })
			.build()
		},
		{"game", UIBuilder()
			.build()
		},
		{"editor", UIBuilder()
			.build()
		}
	};
}

void Game::update()
{
    Keyboard::update();
	Timer::updateAll();
	if(level.isLoaded()) level.update();
	checkUI();
}

void Game::checkUI() {
	if (ui->isButtonPressed("start")) {
		level.loadGame("saves/level.txt");
		ui = uis.at("game");
		level.linkUI(ui, background);
	}
	if (ui->isButtonPressed("editor")) {
		level.loadEditor("saves/level.txt");
		ui = uis.at("editor");
		level.linkUI(ui, background);
	}
	if (ui->isButtonPressed("exitMenu")) {
		level.save();
		level = Level();
		ui = uis.at("startMenu");
		level.linkUI(ui, background);
		background->setSpeed({1, 0});
	}
	if (ui->isButtonPressed("continue")) {
		ui = uis.at("game");
		level.linkUI(ui, background);
	}

	if (Keyboard::isKeyPressed(KEY_ESCAPE)) {
		if (level.isLoaded()) {
			ui = uis.at("pauseMenu");
			level.linkUI(ui, background);
		}
	}
}

void Game::draw() {
	BeginDrawing();

		ClearBackground(background ? background->getColor() : ui->getBackgroundColor());

		background->update();
		if(level.isLoaded()) level.render();
		ui->update();
		
        DrawFPS(10, 10);
        
	EndDrawing();
}