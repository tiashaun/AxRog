#ifndef MAP_H
#define MAP_H

#include <SDL/SDL.h>
#include "room.hpp"
#include "tile.hpp"

class Map {
    public:
    Map(int w, int h);
    ~Map(void);
    Tile* GetTile(int x, int y);
    void DrawTo(SDL_Surface *surf);

    Room *start_room;
    Room *end_room;
    int width;
    int height;
    int cam_x;
    int cam_y;
    Tile *tiles;

    private:
};

#endif
