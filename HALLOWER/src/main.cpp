#include "raylib.h"
#include "iostream"
#include "cmath"
#include "player.h"
#include "AnimatedSprite.h"

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
    Vector2 spriteSize = {24, 24};
    // simple sprite used to test the Animated sprite class
    AnimatedSprite sprite = AnimatedSprite("Art/playerSheet.png", spriteSize);
    sprite.addAnimation("spin", 0, 0, 4, 4);
    sprite.playAnimation("spin");

    Rectangle wall = {320, 320, 64, 64};

    while (!WindowShouldClose())
    {
        float delta = GetFrameTime();

        if (CheckCollisionRecs(player.render, wall))
        {
            player.Colliding();
        }
        // draw everything to the gameRender texture
        BeginTextureMode(gameRender);

        ClearBackground(BLUE);
        DrawRectangleRec(wall, GRAY);
        player.Update();
        sprite.Update();

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