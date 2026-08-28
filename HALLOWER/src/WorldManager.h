#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#include <vector>
#include "raylib.h"
#include "Tile.h"
#include "GrassTile.h"

class WorldManager
{
private:
    GrassTile grassRenderer;

public:
    WorldManager() = default;
    ~WorldManager() = default;
    std::vector<Tile> World;
    void createLevel(const std::vector<int> &levelLayout);
    void drawGrass(std::vector<Tile> &level);
    std::vector<Tile *> getNearbyTiles(Vector2 position);
};

#endif