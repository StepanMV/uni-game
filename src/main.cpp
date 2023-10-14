#include "game.h"
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "style_cyber.h"

int main()
{
    Game game("Low-budget Terraria");
    game.load();

    GuiLoadStyleCyber();

    while (!game.shouldClose()) game.tick();
    return 0;
}