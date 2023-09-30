#include "game.h"

int main()
{
    Game game(1366, 768, 60, "Low-budget Terraria");
    //Game game(1920, 1080, 60, "Low-budget Terraria");
    game.loadLevel();

    while (!game.shouldClose()) game.tick();
    return 0;
}