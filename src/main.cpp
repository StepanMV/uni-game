// im here
#include "raylib.h"
#include "game.h"
#include "player.h"
#include <iostream>

#define GLSL_VERSION 330
#define RAYGUI_IMPLEMENTATION

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    Vector2 mousePos;

    InitWindow(screenWidth, screenHeight, "Learning shaders");
    Texture2D myTexture = LoadTexture("resources/textures/snickers.png"); 

    Shader shader = LoadShader(0, TextFormat("resources/shaders/my_shader.fs"));

    int mousePosLoc = GetShaderLocation(shader, "mousePos");

    SetTargetFPS(60);                           // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------


    // Main game loop

    while (!WindowShouldClose())                // Detect window close button or ESC key
    {
        //----------------------------------------------------------------------------------

        mousePos.x = (float) (GetMouseX()) / screenWidth;
        mousePos.y = (float) (GetMouseY()) / screenHeight;
        SetShaderValue(shader, mousePosLoc, &mousePos.x, SHADER_UNIFORM_VEC2);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);
            
            BeginShaderMode(shader);

                DrawTexture(myTexture, 0, 0, BLACK);

            EndShaderMode();
            DrawFPS(10, 10);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadShader(shader);       // Unload shader
    UnloadTexture(myTexture);

    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}