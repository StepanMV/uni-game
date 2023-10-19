/*******************************************************************************************
*
*   LayoutName v1.0.0 - Tool Description
*
*   LICENSE: Propietary License
*
*   Copyright (c) 2022 raylib technologies. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is proprietary and confidential unless the owner allows
*   usage in any other form by expresely written permission.
*
**********************************************************************************************/

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

//----------------------------------------------------------------------------------
// Controls Functions Declaration
//----------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //---------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "layout_name");

    // layout_name: controls initialization
    //----------------------------------------------------------------------------------
    bool Button000Pressed = false;
    bool Button001Pressed = false;
    bool DropdownBox004EditMode = false;
    int DropdownBox004Active = 0;
    float SliderBar006Value = 0.0f;
    //----------------------------------------------------------------------------------

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Implement required update logic
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))); 

            // raygui: controls drawing
            //----------------------------------------------------------------------------------
            if (DropdownBox004EditMode) GuiLock();

            Button000Pressed = GuiButton((Rectangle){ 968, 744, 312, 72 }, "CONFIRM"); 
            Button001Pressed = GuiButton((Rectangle){ 640, 744, 312, 72 }, "CANCEL"); 
            GuiGroupBox((Rectangle){ 640, 120, 640, 600 }, "SETTINGS");
            GuiStatusBar((Rectangle){ 688, 248, 192, 40 }, "SAMPLE TEXT");
            GuiStatusBar((Rectangle){ 688, 168, 192, 40 }, "SAMPLE TEXT");
            SliderBar006Value = GuiSliderBar((Rectangle){ 960, 168, 288, 40 }, NULL, NULL, SliderBar006Value, 0, 100);
            if (GuiDropdownBox((Rectangle){ 960, 248, 288, 40 }, "1920x1080;1600x900;1280x720", &DropdownBox004Active, DropdownBox004EditMode)) DropdownBox004EditMode = !DropdownBox004EditMode;
            
            GuiUnlock();
            //----------------------------------------------------------------------------------

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Controls Functions Definitions (local)
//------------------------------------------------------------------------------------

