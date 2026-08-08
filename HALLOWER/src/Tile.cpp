#include "raylib.h"
#include "Tile.h"

Tile::Tile()
{
}

Tile::Tile(TYPE type, Rectangle shape)
{
    this->type = type;
    this->shape = shape;
    this->solid = true;
    breakable = false;
    if (type == BURROWABLE)
    {
        burrowable = true;
        jumpable = false;
    }
    else if (type == JUMPABLE)
    {
        burrowable = false;
        jumpable = true;
    }
    else if (type == WALL)
    {
        burrowable = false;
        jumpable = false;
    }
    else if (type == EMPTY)
    {
        solid = false;
    }
}

void Tile::OnHit()
{
    color = DARKBROWN;
    jumpable = true;
}