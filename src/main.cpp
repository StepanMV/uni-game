/*******************************************************************************************
*
*   raylib [shaders] example - Multiple sample2D with default batch system
*
*   NOTE: This example requires raylib OpenGL 3.3 or ES2 versions for shaders support,
*         OpenGL 1.1 does not support shaders, recompile raylib to OpenGL 3.3 version.
*
*   NOTE: Shaders used in this example are #version 330 (OpenGL 3.3), to test this example
*         on OpenGL ES 2.0 platforms (Android, Raspberry Pi, HTML5), use #version 100 shaders
*         raylib comes with shaders ready for both versions, check raylib/shaders install folder
*
*   Example originally created with raylib 3.5, last time updated with raylib 3.5
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2020-2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
// im here

#include "raylib.h"
#include <iostream>

#define GLSL_VERSION 330

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1600;
    const int screenHeight = 758;

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