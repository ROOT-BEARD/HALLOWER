#ifndef TILE_H
#define TILE_H

#include "raylib.h"

class Tile
{
private:
    void updateTile();

public:
    Tile();
    enum TYPE
    {
        WALL,
        JUMPABLE,
        BURROWABLE,
        EMPTY,
        PIT,
        GRASS
    };
    TYPE type;
    bool jumpable;
    bool burrowable;
    bool breakable;
    bool solid;
    Color color;
    Rectangle shape;
    Tile(TYPE type, Rectangle shape);
    void OnHit();
};

#endif