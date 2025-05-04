#pragma once
#include <vector>

const int MAP_WIDTH = 100;
const int MAP_HEIGHT = 100;

enum class TileType { Grass, Water, Mountain, TallGrass, Trees, Border, Unknown };

struct Tile {
    TileType type;
    bool explored = false;
};

struct Position {
    int x, y;
};

class Map {
public:
    std::vector<std::vector<Tile>> grid;

    Map();
    void generateWorld();
    void reveal(Position pos);
    char getTileChar(TileType t) const;
};
