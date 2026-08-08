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
        BURROWABLE,
        EMPTY
    };
    bool jumpable;
    bool burrowable;
    bool breakable;
    bool solid;
    Color color;
    TYPE type;
    Rectangle shape;
    Tile(TYPE type, Rectangle shape);
    void OnHit();
};

#endif TILE_H