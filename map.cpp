#include "map.hpp"
#include <cstdlib>

Map::Map(int w, int h) {
    width = w;
    height = h;

    tiles = new Tile[h*w];
}

Map::~Map(void) {
    delete tiles;
}

Tile*
Map::getTile(int x, int y) {
    int index;

    if (x < 0 || x > width)
        return NULL;
    if (y < 0 || y > height)
        return NULL;
    index = y * width + x;

    return &tiles[index];
}
