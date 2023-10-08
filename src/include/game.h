#pragma once

#include <string>

#include "raylib.h"
#include "level.h"
#include "player.h"
#include "tile.h"
#include "ui.h"

class Game {
public:
	Game(int width, int height, int fps, std::string title);

	Game(const Game& other) = delete;

	Game& operator=(const Game& other) = delete;

	~Game() noexcept;

	bool shouldClose() const;

	void tick();

	void load();

	const std::map<std::string, std::shared_ptr<UI>> uis;

private:
	void createUIS();

	void update();
	void checkUI();
	void draw();

	Level level;
	std::shared_ptr<UI> ui;
    std::shared_ptr<Background> background;
};
