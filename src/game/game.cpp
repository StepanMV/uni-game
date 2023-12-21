#include "game.h"

#include "raylib.h"
#include "player.h"
#include "tile.h"
#include "renderer.h"
#include "controls.h"
#include "ui.h"
#include "particle.h"
#include "audio.h"

#include <vector>
#include <iostream>

const std::shared_ptr<IniFile> Game::settings = std::make_shared<IniFile>("settings.ini");
std::shared_ptr<UI> Game::ui = nullptr;
std::shared_ptr<Background> Game::background = nullptr;
 unsigned int Game::unlockedLevel = 0;

Game::Game(std::string title) {	
	SetTargetFPS(settings->readInt("Screen", "screenRefreshRate", 60));
	SetConfigFlags(FLAG_VSYNC_HINT);
	InitWindow(settings->readInt("Screen", "screenWidth", 1920), settings->readInt("Screen", "screenHeight", 1080), title.c_str());
	if(settings->readBool("Screen", "fullscreen")) ToggleFullscreen();
	SetWindowIcon(LoadImage("resources/textures/Icon.png"));

}

Game::~Game() noexcept
{
	Renderer::unloadTextures();
	Audio::unload();
	settings->deleteSection("Runtime");
	settings->save();
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
	Audio::init("resources/audio");
	Audio::setMasterVolume(settings->readDouble("Sound", "masterVolume", 1.0));
	Audio::setMusicVolume(settings->readDouble("Sound", "musicVolume", 1.0));
	Audio::setSoundVolume(settings->readDouble("Sound", "soundVolume", 1.0));
	unlockedLevel = settings->readInt("Game", "unlockedLevel", 0);
	Controls::init();
	createUIS();

	ui = createLevelSelector();
	background = Background::create(2, 0.25);
	Audio::setMusic("CalamityTitle");
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
			.build()
		},
		{"game", UIBuilder()
			.addBar("healthBar", BarData{ Rectangle{ 120, 960, 216, 24 }, "HEALTH" })
			.addBar("staminaBar", BarData{ Rectangle{ 120, 1008, 216, 24 }, "STAMINA" })
			.addBar("bossHealthBar", BarData{ Rectangle{ 720, 984, 480, 24 }, "BOSS HEALTH" })
			.addSubUI("pauseMenu", pauseMenuUI, false)
			.build()
		},
		{"kitSelection", UIBuilder()
			.build()
		},
		{"editor", UIBuilder()
			.addScrollMenu("tileSelector", Rectangle{ 1650, 60, 240, 960 }, "TILE SELECTOR", editorScrollUI.build())
			.addSubUI("pauseMenu", pauseMenuUI, false)
			.build()
		},
		{"settings" , UIBuilder()
			.addGroupBox("settings", GroupBoxData{ Rectangle{ 640, 120, 640, 600 }, "SETTINGS" })
			.addButton("cancel", ButtonData{ Rectangle{ 640, 744, 312, 72 }, "BACK" })
			.addButton("confirm", ButtonData{ Rectangle{ 968, 744, 312, 72 }, "CONFIRM" })
			.addDummyRect("resolution", DummyRectData{ Rectangle{ 688, 168, 192, 40 }, "RESOLUTION" })
			.addDropdown("resolutionDropdown", DropdownData{ Rectangle{ 960, 168, 288, 40 }, "1920x1080;1600x1200;1440x1080;1600x900;1366x768;1280x960;1280x720;1024x768;800x600" })
			.addDummyRect("fullscreen", DummyRectData{ Rectangle{ 688, 248, 192, 40 }, "FULLSCREEN" })
			.addDropdown("fullscreenDropdown", DropdownData{ Rectangle{ 960, 248, 288, 40 }, "OFF;ON" })
			.addDummyRect("masterVolume", DummyRectData{ Rectangle{ 688, 328, 192, 40 }, "MASTER" })
			.addSliderBar("masterVolumeSlider", SliderBarData{ Rectangle{ 960, 328, 288, 40 }, "", "", 0, 1 })
			.addDummyRect("musicVolume", DummyRectData{ Rectangle{ 688, 408, 192, 40 }, "MUSIC" })
			.addSliderBar("musicVolumeSlider", SliderBarData{ Rectangle{ 960, 408, 288, 40 }, "", "", 0, 1 })
			.addDummyRect("soundVolume", DummyRectData{ Rectangle{ 688, 488, 192, 40 }, "SOUND" })
			.addSliderBar("soundVolumeSlider", SliderBarData{ Rectangle{ 960, 488, 288, 40 }, "", "", 0, 1 })
			.build()
		}
	};
}

std::shared_ptr<UI> Game::createKitSelector() { 
	std::shared_ptr<UI> ui = UIBuilder()
		.addObject("kit1", Particle::createFromObject(Player::spawn((levelID + 1) * 10 + 1, Vec2(480, 400), Vec2(96, 144))))
		.addObject("kit2", Particle::createFromObject(Player::spawn((levelID + 1) * 10 + 2, Vec2(800, 400), Vec2(96, 144))))
		.addObject("kit3", Particle::createFromObject(Player::spawn((levelID + 1) * 10 + 3, Vec2(1120, 400), Vec2(96, 144))))
		.addObject("kit4", Particle::createFromObject(Player::spawn((levelID + 1) * 10 + 4, Vec2(1440, 400), Vec2(96, 144))))
		.addButton("kit1Button", ButtonData{ Rectangle{ 432, 328, 96, 144 }, "", false }, [this]() { kitButtonCallback(1); })
		.addButton("kit2Button", ButtonData{ Rectangle{ 752, 328, 96, 144 }, "", false }, [this]() { kitButtonCallback(2); })
		.addButton("kit3Button", ButtonData{ Rectangle{ 1072, 328, 96, 144 }, "", false }, [this]() { kitButtonCallback(3); })
		.addButton("kit4Button", ButtonData{ Rectangle{ 1392, 328, 96, 144 }, "", false }, [this]() { kitButtonCallback(4); })
		.addDummyRect("chooseLabel", DummyRectData{Rectangle{712, 200, 500, 50}, "Choose class"})
		.addDummyRect("kit1Label", DummyRectData{Rectangle{432, 528, 96, 30}, "Warrior"})
		.addDummyRect("kit2Label", DummyRectData{Rectangle{752, 528, 96, 30}, "Ranger"})
		.addDummyRect("kit3Label", DummyRectData{Rectangle{1072, 528, 96, 30}, "Shooter"})
		.addDummyRect("kit4Label", DummyRectData{Rectangle{1392, 528, 96, 30}, "Mage"})
		.build();
	const_cast<std::map<std::string, std::shared_ptr<UI>>&>(uis)["kitSelection"] = ui;
	return ui;
}

std::shared_ptr<UI> Game::createLevelSelector() {
    auto builder = UIBuilder()
		.addButton("start", ButtonData{ Rectangle{ 840, 432, 240, 48 }, "START" })
		.addButton("exit", ButtonData{ Rectangle{ 840, 768, 240, 48 }, "EXIT" })
		.addButton("editor", ButtonData{ Rectangle{ 840, 504, 240, 48 }, "EDITOR" })
		.addButton("settings", ButtonData{ Rectangle{ 840, 696, 240, 48 }, "SETTINGS" })
		.addDummyRect("enemySelector", DummyRectData{ Rectangle{ 840, 600, 240, 24 }, "ENEMY SELECTOR" });
	switch (unlockedLevel) {
	case 0:
		builder.addDropdown("bossDropdown", DropdownData{ Rectangle{ 840, 624, 240, 48 }, "KING SLIME" });
		break;
	case 1:
		builder.addDropdown("bossDropdown", DropdownData{ Rectangle{ 840, 624, 240, 48 }, "KING SLIME;EYE OF CTHULHU" });
		break;
	case 2:
		builder.addDropdown("bossDropdown", DropdownData{ Rectangle{ 840, 624, 240, 48 }, "KING SLIME;EYE OF CTHULHU;EATER OF WORLDS" });
		break;
	}
	const_cast<std::map<std::string, std::shared_ptr<UI>>&>(uis)["startMenu"] = builder.build();
	return uis.at("startMenu");
}

void Game::update()
{
    Controls::update();
	Timer::updateAll();
	Audio::update();
	if(level.isLoaded()) level.update();
	checkUI();
}

Vec2 intToRes(int dropdown) {
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

int resToInt(Vec2 res) {
	if (res == Vec2{ 1920, 1080 }) return 0;
	if (res == Vec2{ 1600, 1200 }) return 1;
	if (res == Vec2{ 1440, 1080 }) return 2;
	if (res == Vec2{ 1600, 900 }) return 3;
	if (res == Vec2{ 1366, 768 }) return 4;
	if (res == Vec2{ 1280, 960 }) return 5;
	if (res == Vec2{ 1280, 720 }) return 6;
	if (res == Vec2{ 1024, 768 }) return 7;
	if (res == Vec2{ 800, 600 }) return 8;
	return 0;
}

void Game::checkUI() {
	if (ui->isButtonPressed("start")) {
		levelID = ui->getDropdownValue("bossDropdown");
		ui = createKitSelector();
		ui->update();
	}
	if (ui->isButtonPressed("editor")) {
		levelID = ui->getDropdownValue("bossDropdown");
		level.loadEditor("saves/level.txt", levelID);
		ui = uis.at("editor");
		background = Background::create(1);
		ui->update();
	}
	if (ui->isButtonPressed("settings")) {
		Vec2 currentRes = { (float) settings->readInt("Screen", "screenWidth"), (float) settings->readInt("Screen", "screenHeight") };
		ui = uis.at("settings");
		ui->setSliderValue("masterVolumeSlider", Audio::getMasterVolume());
		ui->setSliderValue("musicVolumeSlider", Audio::getMusicVolume());
		ui->setSliderValue("soundVolumeSlider", Audio::getSoundVolume());
		ui->setDropdownValue("fullscreenDropdown", settings->readBool("Screen", "fullscreen"));
		ui->setDropdownValue("resolutionDropdown", resToInt(currentRes));

		ui->update();
	}
	if (ui->isButtonPressed("exit")) {
		close = true;
	}
	if (ui->isButtonPressed("cancel")) {
		ui = createLevelSelector();
		ui->update();
	}

	if (ui->isButtonPressed("confirm")) {
		Vec2 res = intToRes(ui->getDropdownValue("resolutionDropdown"));
		if (ui->getDropdownValue("fullscreenDropdown") != Game::settings->readBool("Screen", "fullscreen")) {
			ToggleFullscreen();
			Game::settings->writeBool("Screen", "fullscreen", ui->getDropdownValue("fullscreenDropdown"));
		}
		Game::settings->writeInt("Screen", "screenWidth", res.x);
		Game::settings->writeInt("Screen", "screenHeight", res.y);
		Audio::setMasterVolume(ui->getSliderValue("masterVolumeSlider"));
		Audio::setMusicVolume(ui->getSliderValue("musicVolumeSlider"));
		Audio::setSoundVolume(ui->getSliderValue("soundVolumeSlider"));
		settings->writeDouble("Sound", "masterVolume", ui->getSliderValue("masterVolumeSlider"));
		settings->writeDouble("Sound", "musicVolume", ui->getSliderValue("musicVolumeSlider"));
		settings->writeDouble("Sound", "soundVolume", ui->getSliderValue("soundVolumeSlider"));
	}

	if (Controls::isKeyPressed(KEY_ESCAPE)) {
		if (auto pauseMenu = ui->getSubUI("pauseMenu")) {
			Audio::playSound("Menu_Tick");
			pauseMenu->setEnabled(true);
		} else if (ui == uis.at("kitSelection")) {
			Audio::playSound("Menu_Tick");
			ui = createLevelSelector();
			ui->update();
		}
	}
	if (auto pauseMenu = ui->getSubUI("pauseMenu")) {
		if (pauseMenu->isButtonPressed("exitMenu")) {
			level.save();
			level.unload();
			ui = createLevelSelector();
			ui->update();
			pauseMenu->setEnabled(false);
			background = Background::create(2, 0.25);
			Audio::setMusic("CalamityTitle");
		}
		if (pauseMenu->isButtonPressed("continue")) {
			pauseMenu->setEnabled(false);
		}
		if (pauseMenu->isButtonPressed("exit")) {
			close = true;
		}
	}
	if (ui == uis.at("kitSelection")) {
		for (int i = 1; i <= 4; ++i) {
			std::string kitName = "kit" + std::to_string(i);
			if (ui->isInsideElement(Controls::getMousePos(), kitName)) {
				auto obj = ui->getObjectRenderer(kitName);
				obj->setState("moving");
			} else {
				auto obj = ui->getObjectRenderer(kitName);
				obj->setState("idle");
			}
		}
	}
}

void Game::draw() {
	BeginDrawing();

		ClearBackground(background ? background->getColor() : ui->getBackgroundColor());
		background->update();
		if(level.isLoaded()) level.render();
		ui->update();
		
        // DrawFPS(10, 10);
        
	EndDrawing();
}

void Game::kitButtonCallback(unsigned id) {
	unsigned kitID = (levelID + 1) * 10 + id;
	level.loadGame("saves/level.txt", levelID, kitID);
	background = Background::create(1);
	Audio::setMusic("BossRushTier2");
	ui = uis.at("game");
	ui->update();
}
