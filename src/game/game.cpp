#include "Game.h"

#include "raylib.h"
#include "player.h"
#include "tile.h"
#include "renderer.h"
#include "keyboard.h"
#include "ui.h"

#include <vector>
#include <iostream>

const std::shared_ptr<IniFile> Game::settings = std::make_shared<IniFile>("settings.ini");
std::shared_ptr<UI> Game::ui = nullptr;
std::shared_ptr<Background> Game::background = nullptr;
std::shared_ptr<CoolCamera> Game::camera = nullptr;

Game::Game(std::string title) {	
	SetTargetFPS(settings->readInt("Screen", "screenRefreshRate", 60));
	InitWindow(settings->readInt("Screen", "screenWidth", 1920), settings->readInt("Screen", "screenHeight", 1080), title.c_str());
	if(settings->readBool("Screen", "fullscreen")) ToggleFullscreen();
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
	Game::settings->writeDouble("Runtime", "screenCoefW", Game::settings->readDouble("Screen", "screenWidth", 1920) / 1920.0);
	Game::settings->writeDouble("Runtime", "screenCoefH", Game::settings->readDouble("Screen", "screenHeight", 1080) / 1080.0);
	Renderer::loadTextures("resources/textures");
	Keyboard::init();
	createUIS();

	ui = uis.at("startMenu");
	background = Background::create(1, 0.25);
	camera = CoolCamera::init();
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
			.addBar("healthBar", BarData{ Rectangle{ 120, 960, 216, 24 }, "HEALTH", nullptr, 0, 100 })
			.addBar("staminaBar", BarData{ Rectangle{ 120, 1008, 216, 24 }, "STAMINA", nullptr, 0, 100 })
			.addBar("bossHealthBar", BarData{ Rectangle{ 720, 984, 480, 24 }, "BOSS HEALTH", nullptr, 0, 100 })
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
	camera->update();
	checkUI();
}

void Game::checkUI() {
	if (ui->isButtonPressed("start")) {
		level.loadGame("saves/level.txt");
		ui = uis.at("game");
	}
	if (ui->isButtonPressed("editor")) {
		level.loadEditor("saves/level.txt");
		ui = uis.at("editor");
	}
	if (ui->isButtonPressed("exitMenu")) {
		level.save();
		level = Level();
		ui = uis.at("startMenu");
		background->setSpeed({1, 0});
	}
	if (ui->isButtonPressed("continue")) {
		ui = uis.at("game");
	}

	if (Keyboard::isKeyPressed(KEY_ESCAPE)) {
		if (level.isLoaded()) {
			ui = uis.at("pauseMenu");
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