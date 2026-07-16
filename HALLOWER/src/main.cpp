#include "raylib.h"
#include "iostream"
#include "cmath"
#include "player.h"
#include "AnimatedSprite.h"

int main()
{
    // Testing github commit
    InitWindow(640, 640, "GAME");
    SetTargetFPS(60);

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
        BeginDrawing();

        ClearBackground(BLUE);

        DrawRectangleRec(wall, GRAY);
        player.Update();
        sprite.Update();

        EndDrawing();
    }
    CloseWindow();
    return 0;
}