#include <iostream>
#include <cmath>
#include <vector>

#include "raylib.h"
#include "Player.h"
#include "AnimatedSprite.h"
#include "WorldManager.h"
#include "time.h"
#include "Tile.h"

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

    Player player;
    WorldManager manager;
    player.playerPos = (Vector2){4 * 16.0f, 4 * 16.0f};

    Camera2D camera = {0};
    camera.offset = (Vector2){windowSize.x / 2.0f, windowSize.y / 2.0f};
    camera.target.y = windowSize.y / 2.0f;
    camera.zoom = 1.0f;

    std::vector<int> level = {// Row 0
                              2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                              // Row 1
                              2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 2,
                              // Row 2
                              2, 0, 4, 4, 4, 0, 0, 2, 0, 1, 1, 1, 1, 0, 0, 2,
                              // Row 3
                              2, 0, 4, 4, 4, 0, 0, 2, 0, 0, 0, 0, 1, 0, 0, 2,
                              // Row 4
                              2, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 1, 0, 0, 2,
                              // Row 5
                              2, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
                              // Row 6
                              2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 0, 2,
                              // Row 7
                              2, 2, 2, 2, 0, 0, 2, 2, 2, 0, 3, 3, 3, 3, 0, 2,
                              // Row 8
                              2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2,
                              // Row 9
                              2, 0, 1, 1, 0, 0, 0, 0, 2, 0, 1, 1, 1, 0, 0, 2,
                              // Row 10
                              2, 0, 0, 0, 0, 3, 3, 0, 2, 0, 0, 0, 0, 0, 0, 2,
                              // Row 11
                              2, 0, 0, 0, 0, 3, 3, 0, 2, 2, 2, 0, 0, 2, 2, 2,
                              // Row 12
                              2, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
                              // Row 13
                              2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
                              // Row 14
                              2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
    manager.createLevel(level);

    while (!WindowShouldClose())
    {
        float delta = GetFrameTime();

        player.Update();
        player.nearbyTiles = manager.getNearbyTiles({player.collision.x, player.collision.y});

        camera.target.x = player.playerPos.x;

        // draw everything to the gameRender texture
        BeginTextureMode(gameRender);

        ClearBackground(BLUE);

        BeginMode2D(camera);

        manager.drawLevel(manager.World);
        player.Draw();

        EndMode2D();

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