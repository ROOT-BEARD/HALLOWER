#ifndef TILE_H
#define TILE_H

#include "raylib.h"

class Tile
{
private:
    /* data */
public:
    Tile();
    enum TYPE
    {
        WALL,
        JUMPABLE,
        BURROWABLE
    };
    bool jumpable;
    bool burrowable;
    bool breakable;
    Color color;
    TYPE type;
    Rectangle shape;
    Tile(TYPE type, Rectangle shape);
    void OnHit();
};

#endif TILE_H