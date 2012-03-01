#ifndef MAP_H
#define MAP_H

#include "room.hpp"
#include "tile.hpp"

class Map {
    public:
    Map(int w, int h);
    ~Map(void);
    Tile* getTile(int x, int y);

    Room *start_room;
    Room *end_room;
    int width;
    int height;
    Tile *tiles;

    private:
};

#endif
