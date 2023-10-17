#define RAYGUI_IMPLEMENTATION
#include "game.h"
#include "raylib.h"
#include "raygui.h"

int main()
{
    Game game("Low-budget Terraria");
    game.load();

    while (!game.shouldClose()) game.tick();
    return 0;
}