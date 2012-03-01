#ifndef TILE_H
#define TILE_H

#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <string>
#include "resource.hpp"

class Tile {
    public:
    Tile(void);
    void MakeWall(void);
    void MakeFloor(void);

    bool isUnusedSpace;
    SDL_Surface *surf;
    bool visible;
};

#endif
