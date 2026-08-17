#include <iostream>
#include <cmath>
#include <vector>

#include "raylib.h"
#include "Player.h"
#include "AnimatedSprite.h"
#include "WorldManager.h"
#include "time.h"
#include "Tile.h"
#include "GrassTile.h"

int main()
{
    // the diminsions of the window in pixels
    Vector2 windowSize = {256, 240};
    // how much the window is scaled by
    int windowScale = 5;
    // the window is intitatied with the dimisions of the windowSize * the window scale
    InitWindow((windowSize.x * windowScale), (windowSize.y * windowScale), "GAME");
    SetTargetFPS(60);
    // RenderTexture that is drawen to and scaled up to fit the screen
    RenderTexture2D gameRender = LoadRenderTexture(windowSize.x, windowSize.y);

    std::string bg = "Art/background.png";
    Texture2D backGround = LoadTexture(bg.c_str());
    std::string fg = "Art/foreground.png";
    Texture2D foreGround = LoadTexture(fg.c_str());

    Player player;
    WorldManager manager;
    player.playerPos = {2 * 16.0f, 2 * 16.0f};
    float delta = GetFrameTime();

    std::vector<int> level = {// Row 0
                              2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                              // Row 1
                              2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                              // Row 2
                              0, 0, 0, 2, 5, 4, 4, 4, 4, 0, 0, 2, 2, 2, 2, 2,
                              // Row 3
                              0, 0, 0, 3, 5, 4, 4, 4, 4, 0, 0, 2, 2, 2, 2, 2,
                              // Row 4
                              0, 0, 0, 3, 5, 4, 4, 4, 4, 0, 0, 1, 0, 2, 2, 2,
                              // Row 5
                              0, 0, 0, 3, 5, 0, 0, 0, 0, 0, 0, 1, 0, 2, 2, 2,
                              // Row 6
                              0, 0, 0, 3, 5, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2,
                              // Row 7
                              0, 0, 0, 3, 5, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2,
                              // Row 8
                              0, 0, 0, 2, 5, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0,
                              // Row 9
                              2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0,
                              // Row 10
                              2, 2, 2, 2, 2, 2, 4, 0, 0, 0, 0, 0, 5, 5, 0, 0,
                              // Row 11
                              2, 2, 2, 2, 2, 2, 4, 4, 4, 0, 0, 0, 5, 5, 0, 0,
                              // Row 12
                              2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                              // Row 13
                              2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                              // Row 14
                              2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
    manager.createLevel(level);

    while (!WindowShouldClose())
    {
        delta = GetFrameTime();

        player.Update(delta);
        player.nearbyTiles = manager.getNearbyTiles({player.collision.x, player.collision.y});

        // draw everything to the gameRender texture
        BeginTextureMode(gameRender);

        ClearBackground(BLUE);

        DrawTexture(backGround, 0, 0, WHITE);
        manager.drawGrass(manager.World);
        player.Draw();

        DrawTexture(foreGround, 0, 0, WHITE);

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
    UnloadRenderTexture(gameRender);
    CloseWindow();
    return 0;
}