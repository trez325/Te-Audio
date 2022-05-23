#include "raylib.h"
#include <iostream>
void setting()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 300;
    const int screenHeight = 550;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    Image imsettings = LoadImage("C:\\Users\\trez325\\Pictures\\Settings2.png");
    Texture2D texture = LoadTextureFromImage(imsettings);
    Vector2 position = {0, 0};
    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    std::cout << "dee";
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawTextureV(texture, position, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

}