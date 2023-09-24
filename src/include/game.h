#pragma once

#include <string>

#include "raylib.h"
#include "level.h"
#include "player.h"
#include "tile.h"

class Game {
public:
	Game(int width, int height, int fps, std::string title);

	Game(const Game& other) = delete;

	Game& operator=(const Game& other) = delete;

	~Game() noexcept;

	bool shouldClose() const;

	void tick();

	void loadLevel();

private:

	void update();

	void draw();

	void checkCollisions();

    //Level level;
    Camera2D camera;
	Player player;
	Texture2D background;
	std::vector<Tile> tiles;
};
