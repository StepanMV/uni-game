#pragma once

#include <string>

#include "raylib.h"
#include "level.h"

class Game {
public:
	Game(int width, int height, int fps, std::string title);

	Game(const Game& other) = delete;

	Game& operator=(const Game& other) = delete;

	~Game() noexcept;

	bool shouldClose() const;

	void tick();

private:
	void update();

	void draw();

    Level level;
    Camera2D camera;

};
