#include "raylib.h"
#include "iostream"
#include "cmath"
#include "player.h"
#include "AnimatedSprite.h"
#include "vector"

int main()
{
    // the diminsions of the window in pixels
    Vector2 windowSize = {256, 240};
    // how much the window is scaled by
    int windowScale = 4;
    // the window is intitatied with the dimisions of the windowSize * the window scale
    InitWindow((windowSize.x * windowScale), (windowSize.y * windowScale), "GAME");
    SetTargetFPS(60);
    // RenderTexture that is drawen to and scaled up to fit the screen
    RenderTexture2D gameRender = LoadRenderTexture(windowSize.x, windowSize.y);

    Player player;

    Rectangle wall = {windowSize.x / 2, windowSize.y / 2, 16, 16};
    Rectangle wall2 = {windowSize.x / 2 + 16, windowSize.y / 2, 16, 16};

    player.level.push_back(wall);
    player.level.push_back(wall2);

    while (!WindowShouldClose())
    {
        float delta = GetFrameTime();

        if (CheckCollisionRecs(player.collision, wall))
        {
            player.Colliding();
        }
        // draw everything to the gameRender texture
        BeginTextureMode(gameRender);

        ClearBackground(BLUE);
        DrawRectangleRec(wall, GREEN);
        DrawRectangleRec(wall2, RED);
        player.Update();
        player.Draw();

        EndTextureMode();

        // in the drawing mode draw the gameRender texture
        BeginDrawing();
        /*DrawTexturePro(texture, source, dest, rotation, color), the source is equal to the pixel dimensions defined earlier,
        and the dest is equal to actual window dimensions, scaling the gameRender up*/
        DrawTexturePro(gameRender.texture,
                       Rectangle{0.0f, 0.0f, (float)gameRender.texture.width, (float)-gameRender.texture.height},
                       Rectangle{0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()},
                       Vector2{0, 0},
                       0.0f,
                       WHITE);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}