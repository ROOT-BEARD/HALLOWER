#include "raylib.h"
#include "Tile.h"
#include "AnimatedSprite.h"

Tile::Tile()
{
}

void Tile::updateTile()
{
    switch (type)
    {
    case BURROWABLE:
        solid = true;
        burrowable = true;
        jumpable = false;
        break;
    case JUMPABLE:
        solid = true;
        burrowable = false;
        jumpable = true;
        break;
    case WALL:
        solid = true;
        burrowable = false;
        jumpable = false;
        break;
    case EMPTY:
        solid = false;
        burrowable = false;
        break;
    case PIT:
        jumpable = true;
        burrowable = false;
        solid = false;
        break;
    case GRASS:
        burrowable = false;
        jumpable = false;
        breakable = true;
        solid = true;
    default:
        break;
    }
}

Tile::Tile(TYPE type, Rectangle shape)
{
    this->type = type;
    this->shape = shape;
    updateTile();
}

void Tile::OnHit()
{
    type = EMPTY;
    updateTile();
}