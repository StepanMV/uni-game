// im here
#include "raylib.h"
#include "game.h"

#define GLSL_VERSION 330

int main()
{
    Game game = Game(1280, 720, 60, "Low-budget Terraria");

    while (!game.shouldClose()) {
        game.tick();
    }

    return 0;
}