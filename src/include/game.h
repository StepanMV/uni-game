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

private:

	void update();

	void draw();

	Level level;
	UI ui;
};
