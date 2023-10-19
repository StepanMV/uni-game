#pragma once

#include <string>

#include "raylib.h"
#include "level.h"
#include "player.h"
#include "tile.h"
#include "ui.h"
#include "ini_file.h"

class Game {
public:
	Game(std::string title);

	Game(const Game& other) = delete;

	Game& operator=(const Game& other) = delete;

	~Game() noexcept;

	bool shouldClose() const;

	void tick();

	void load();

	const std::map<std::string, std::shared_ptr<UI>> uis;
	const static std::shared_ptr<IniFile> settings;
	static std::shared_ptr<UI> ui;
	static std::shared_ptr<Background> background;

private:
	bool close = false;
	void createUIS();

	void update();
	void checkUI();
	void draw();

	Level level;
};
