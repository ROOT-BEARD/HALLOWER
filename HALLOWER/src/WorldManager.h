#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#include <vector>
#include "raylib.h"
#include "Tile.h"

class WorldManager
{
public:
    WorldManager(/* args */);
    std::vector<Tile> World;
    void createLevel(std::vector<int> &levelLayout);
    void drawLevel(std::vector<Tile> &level);
    std::vector<Tile *> getNearbyTiles(Vector2 position);
};

#endif