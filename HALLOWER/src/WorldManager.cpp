#include "raylib.h"
#include "vector"
#include "WorldManager.h"
#include "Tile.h"

WorldManager::WorldManager()
{
    //
}

// creates the level based off an int array
std::vector<Tile> WorldManager::createLevel(std::vector<int> levelLayout)
{
    std::vector<Tile> level;
    int y = 0;
    for (int x = 0; x < levelLayout.size(); x++)
    {
        if (x != 0 && x % 16 == 0)
            y++;
        if (levelLayout[x] != 0)
        {
            Tile::TYPE type = Tile::WALL;
            Color color = BLACK;

            switch (levelLayout[x])
            {
            case 1:
                type = Tile::JUMPABLE;
                color = ORANGE;
                break;
            case 2:
                type = Tile::WALL;
                color = BLACK;
                break;
            case 3:
                type = Tile::BURROWABLE;
                color = RED;
                break;

            default:
                break;
            }

            Rectangle shape = {(float)((x % 16) * 16), (float)(y * 16), 16.0f, 16.0f};
            Tile newTile(type, shape);
            newTile.color = color;
            level.push_back(newTile);
        }
    }
    return level;
}

void WorldManager::drawLevel(std::vector<Tile> level)
{
    for (Tile tile : level)
    {
        DrawRectangleRec(tile.shape, tile.color);
    }
}