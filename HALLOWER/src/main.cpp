#include "raylib.h"
#include "iostream"
#include "cmath"
#include "player.h"

int main(){
    InitWindow(640,640, "GAME");
    SetTargetFPS(60);

    Player player;
    Rectangle wall = { 320, 320, 64, 64 };


    while(!WindowShouldClose()){
        float delta = GetFrameTime();
        BeginDrawing();

        if(CheckCollisionRecs(player.render, wall)){
            player.Colliding();
        }

        ClearBackground(BLUE);
        DrawRectangleRec(wall, GRAY);
        player.Update();

        EndDrawing();
    }
    CloseWindow();
    return 0;
}