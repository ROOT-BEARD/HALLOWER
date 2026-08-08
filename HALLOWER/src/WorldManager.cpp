#include <vector>

#include "raylib.h"
#include "WorldManager.h"
#include "Tile.h"

WorldManager::WorldManager()
{
    //
}

// creates the level based off an int array
void WorldManager::createLevel(std::vector<int> &levelLayout)
{
    World.clear();

    int y = 0;
    for (int x = 0; x < levelLayout.size(); x++)
    {
        /*every time that x moudule 16 is equal to 0, besides
        on the first run of the loop, the y index is increased
        to create a new row*/
        if (x != 0 && x % 16 == 0)
            y++;
        // defualt type and color of the tile
        Tile::TYPE type = Tile::WALL;
        Color color = BLACK;
        bool breakable = false;

        // changes the tiles attributes based of the indexes value
        switch (levelLayout[x])
        {
        // 0 = empty tile
        case 0:
            type = Tile::EMPTY;
            color = DARKBLUE;
            break;
        // 1 = jumpable tile
        case 1:
            type = Tile::JUMPABLE;
            color = ORANGE;
            break;
        // 2 = wall tile
        case 2:
            type = Tile::WALL;
            color = BLACK;
            break;
        // 3 = burrowable tile
        case 3:
            type = Tile::BURROWABLE;
            color = RED;
            break;
        // 4 = wall tile that is breakable
        case 4:
            type = Tile::WALL;
            color = BEIGE;
            breakable = true;
            break;

        default:
            break;
        }
        /*create the tiles shape and position based of the current x of the for loop*/
        Rectangle shape = {(float)((x % 16) * 16), (float)(y * 16), 16.0f, 16.0f};
        // create a new tile of the type and shape
        Tile newTile(type, shape);
        // set the color and breakable flag
        newTile.color = color;
        newTile.breakable = breakable;
        // add the tile to the world vector
        World.push_back(newTile);
    }
}

// draws the level based on the vector of tiles
void WorldManager::drawLevel(std::vector<Tile> &level)
{
    // loops trough each tile of the passed in vector and draw it based on the tile shape and color
    for (Tile tile : level)
    {
        DrawRectangleRec(tile.shape, tile.color);
    }
}

// used to get the tiles that are nearby a Vector2 position
std::vector<Tile *> WorldManager::getNearbyTiles(Vector2 position)
{
    // a vectors of pointers to tiles used to hold the tiles nearby
    std::vector<Tile *> nearbyTiles;
    /* gets the minimum and maximum x and y positions, for example,
    the min pos is the starting tile of the nearby tiles on the
    corrasponding axis, and the max is the end tile.*/
    int minXPos = (int)(position.x / 16) - 1;
    int minYPos = (int)(position.y / 16) - 1;
    /*for the max use + 15 is used instead of 16, which is the tile
    height/width, to keep an extra tile from being picked up*/
    int maxXPos = (int)((position.x + 15) / 16) + 1;
    int maxYPos = (int)((position.y + 15) / 16) + 1;
    // check the y columns
    for (int y = minYPos; y <= maxYPos; y++)
    {
        // check the x columns
        for (int x = minXPos; x <= maxXPos; x++)
        {
            // make sure that the tiles are in bounds
            if (x >= 0 && x < 16 && y >= 0 && y < 15)
            {
                // if so, get the index
                int index = (y * 16) + x;
                // add a pointer of the world at the index to nearby tiles
                nearbyTiles.push_back(&World[index]);
            }
        }
    }
    // return all tiles in range
    return nearbyTiles;
}