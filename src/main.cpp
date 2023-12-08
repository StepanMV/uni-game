#define RAYGUI_IMPLEMENTATION
#include "game.h"
#include "raylib.h"
#include "raygui.h"

int main()
{
    Game game("Terraria BossRush");
    game.load();

    while (!game.shouldClose()) game.tick();
    return 0;
}