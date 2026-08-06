#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#include <vector>
#include "raylib.h"
#include "Tile.h"

class WorldManager
{
public:
    WorldManager(/* args */);
    std::vector<Tile> createLevel(std::vector<int> levelLayout);
    void drawLevel(std::vector<Tile> level);
};

#endif