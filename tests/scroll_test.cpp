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
#include "style_cyber.h"
#include <iostream>
#include "rlgl.h"

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
    int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "layout_name");
    rlPushMatrix();

    // layout_name: controls initialization
    //----------------------------------------------------------------------------------
    Rectangle ScrollPanel007ScrollView = {30, 30, 30, 30};
    Vector2 ScrollPanel007ScrollOffset = {0, 0};
    Vector2 ScrollPanel007BoundsOffset = {0, 0};
    //----------------------------------------------------------------------------------

    SetTargetFPS(60);

    GuiLoadStyleCyber();
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
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
        GuiGroupBox((Rectangle){1536, 72, 200, 688}, "BUILDING MENU");
        GuiPanel((Rectangle){1560, 112, 64, 64}, "TEST");
        GuiPanel((Rectangle){1648, 112, 64, 64}, NULL);
        GuiPanel((Rectangle){1560, 200, 64, 64}, NULL);
        GuiPanel((Rectangle){1648, 200, 64, 64}, NULL);
        GuiPanel((Rectangle){1560, 288, 64, 64}, NULL);
        GuiPanel((Rectangle){1648, 288, 64, 64}, NULL);
        std::cout << GetMouseWheelMoveV().x << " " << GetMouseWheelMoveV().y << std::endl;
        GuiScrollPanel(
            (Rectangle){1296, 50, 248 - ScrollPanel007BoundsOffset.x, 300 - ScrollPanel007BoundsOffset.y},
            "Test scroll panel",
            (Rectangle){1296, 50, 248, 900},
            &ScrollPanel007ScrollOffset,
            &ScrollPanel007ScrollView);
        GuiLabel((Rectangle){1328 - ScrollPanel007ScrollOffset.x, 384 - ScrollPanel007ScrollOffset.y, 120, 24}, "SAMPLE TEXT");
        //----------------------------------------------------------------------------------

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Controls Functions Definitions (local)
//------------------------------------------------------------------------------------