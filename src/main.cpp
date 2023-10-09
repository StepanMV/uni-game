#include "game.h"
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "style_cyber.h"

int main()
{
    Game game(1366, 720, 60, "Low-budget Terraria");
    //Game game(1920, 1080, 60, "Low-budget Terraria");
    game.load();

    GuiLoadStyleCyber();

    while (!game.shouldClose()) game.tick();
    return 0;
}