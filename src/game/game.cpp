#include "Game.h"

#include "raylib.h"
#include "player.h"
#include "tile.h"
#include "renderer.h"
#include "controls.h"
#include "ui.h"
#include "particle.h"

#include <vector>
#include <iostream>

const std::shared_ptr<IniFile> Game::settings = std::make_shared<IniFile>("settings.ini");
std::shared_ptr<UI> Game::ui = nullptr;
std::shared_ptr<Background> Game::background = nullptr;

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
	return close || Controls::isKeyPressed(KEY_F4) && Controls::isKeyDown(KEY_LEFT_ALT);
}

void Game::tick() {
	this->update();
	this->draw();
	
}

void Game::load() {

	Game::settings->writeDouble("Runtime", "screenCoefW", GetScreenWidth() / 1920.0);
	Game::settings->writeDouble("Runtime", "screenCoefH", GetScreenHeight() / 1080.0);
	Renderer::loadTextures("resources/textures");
	Controls::init();
	createUIS();

	ui = uis.at("startMenu");
	background = Background::create(1, 0.25);
}

void Game::createUIS() {
	auto editorScrollUI = UIBuilder();
	for (int i = 1; i <= 94; ++i) {
		editorScrollUI.addButton("tile_" + std::to_string(i),
			ButtonData{ Rectangle{ 1650.0f + 32.0f + 104 * ((i - 1) % 2), 60.0f + 32.0f + 104 * ((i - 1) / 2), 72.0f, 72.0f }, "" });
		editorScrollUI.addObject("tile_" + std::to_string(i),
			Particle::createFromObject(TileBuilder::spawn(
				i,
				{ 1650.0f + 32.0f + 104 * ((i - 1) % 2) + 36, 60.0f + 32.0f + 104 * ((i - 1) / 2) + 36 },
				{ 72.0f, 72.0f }
			).build()));
	}
	auto pauseMenuUI = UIBuilder()
		.addButton("continue", ButtonData{ Rectangle{ 792, 312, 336, 48 }, "CONTINUE" })
		.addButton("exitMenu", ButtonData{ Rectangle{ 792, 408, 336, 48 }, "EXIT TO MENU" })
		.addButton("exit", ButtonData{ Rectangle{ 792, 504, 336, 48 }, "EXIT" })
		.build();
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
		{"game", UIBuilder()
			.addBar("healthBar", BarData{ Rectangle{ 120, 960, 216, 24 }, "HEALTH", nullptr, 0, 100 })
			.addBar("staminaBar", BarData{ Rectangle{ 120, 1008, 216, 24 }, "STAMINA", nullptr, 0, 100 })
			.addBar("bossHealthBar", BarData{ Rectangle{ 720, 984, 480, 24 }, "BOSS HEALTH", nullptr, 0, 100 })
			.addSubUI("pauseMenu", pauseMenuUI, false)
			.build()
		},
		{"editor", UIBuilder()
			.addScrollMenu("tileSelector", Rectangle{ 1650, 60, 240, 960 }, "TILE SELECTOR", editorScrollUI.build())
			.addSubUI("pauseMenu", pauseMenuUI, false)
			.build()
		},
		{"settings" , UIBuilder()
			.addGroupBox("settings", GroupBoxData{ Rectangle{ 640, 120, 640, 600 }, "SETTINGS" })
			.addButton("cancel", ButtonData{ Rectangle{ 640, 744, 312, 72 }, "CANCEL" })
			.addButton("confirm", ButtonData{ Rectangle{ 968, 744, 312, 72 }, "CONFIRM (RESTART)" })
			.addDummyRect("resolution", DummyRectData{ Rectangle{ 688, 168, 192, 40 }, "RESOLUTION" })
			.addDropdown("resolutionDropdown", DropdownData{ Rectangle{ 960, 168, 288, 40 }, "1920x1080;1600x1200;1440x1080;1600x900;1366x768;1280x960;1280x720;1024x768;800x600" })
			.addDummyRect("fullscreen", DummyRectData{ Rectangle{ 688, 248, 192, 40 }, "FULLSCREEN" })
			.addDropdown("fullscreenDropdown", DropdownData{ Rectangle{ 960, 248, 288, 40 }, "OFF;ON" })
			.build()
		}
	};
}

void Game::update()
{
    Controls::update();
	Timer::updateAll();
	if(level.isLoaded()) level.update();
	checkUI();
}

Vec2 resSwitch(int dropdown) {
	switch (dropdown) {
	case 0:
		return { 1920, 1080 };
	case 1:
		return { 1600, 1200 };
	case 2:
		return { 1440, 1080 };
	case 3:
		return { 1600, 900 };
	case 4:
		return { 1366, 768 };
	case 5:
		return { 1280, 960 };
	case 6:
		return { 1280, 720 };
	case 7:
		return { 1024, 768 };
	case 8:
		return { 800, 600 };
	default:
		return { 1920, 1080 };
	}
}

void Game::checkUI() {
	if (ui->isButtonPressed("start")) {
		unsigned levelID = ui->getDropdownValue("bossDropdown");
		level.loadGame("saves/level" + std::to_string(levelID) + ".txt", levelID);
		ui = uis.at("game");
		ui->update();
	}
	if (ui->isButtonPressed("editor")) {
		unsigned levelID = ui->getDropdownValue("bossDropdown");
		level.loadEditor("saves/level" + std::to_string(levelID) + ".txt", levelID);
		ui = uis.at("editor");
		ui->update();
	}
	if (ui->isButtonPressed("settings")) {
		ui = uis.at("settings");
		ui->update();
	}
	if (ui->isButtonPressed("exit")) {
		close = true;
	}
	if (ui->isButtonPressed("cancel")) {
		ui = uis.at("startMenu");
		ui->update();
	}

	if (ui->isButtonPressed("confirm")) {
		Vec2 res = resSwitch(ui->getDropdownValue("resolutionDropdown"));
		std::cout << ui->getDropdownValue("resolutionDropdown") << " " << res.x << " " << res.y << std::endl;
		Game::settings->writeInt("Screen", "screenWidth", res.x);
		Game::settings->writeInt("Screen", "screenHeight", res.y);
		Game::settings->writeBool("Screen", "fullscreen", ui->getDropdownValue("fullscreenDropdown"));
		close = true;
	}

	if (Controls::isKeyPressed(KEY_ESCAPE)) {
		if (auto pauseMenu = ui->getSubUI("pauseMenu")) pauseMenu->setEnabled(true);
	}
	if (auto pauseMenu = ui->getSubUI("pauseMenu")) {
		if (pauseMenu->isButtonPressed("exitMenu")) {
			level.save();
			level.unload();
			ui = uis.at("startMenu");
			ui->update();
			pauseMenu->setEnabled(false);
			background->setSpeed({1, 0});
		}
		if (pauseMenu->isButtonPressed("continue")) {
			pauseMenu->setEnabled(false);
		}
		if (pauseMenu->isButtonPressed("exit")) {
			close = true;
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