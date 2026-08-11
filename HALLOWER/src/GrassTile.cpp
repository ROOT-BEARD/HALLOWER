#include "raylib.h"
#include "GrassTile.h"

GrassTile::GrassTile()
{
    render = LoadTexture("Art/grass.png");
    position = {0, 0};
}

void GrassTile::Draw()
{
    DrawTexture(render, position.x, position.y, WHITE);
}