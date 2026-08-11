#ifndef GRASSTILE_H
#define GRASSTILE_H

#include "raylib.h"

class GrassTile
{
public:
    Texture2D render;
    Vector2 position;
    GrassTile();
    void Draw();
};

#endif