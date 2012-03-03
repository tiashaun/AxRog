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
    ~Tile(void);
    void MakeWall(void);
    void MakeFloor(void);
    void MakeDoor(void);

    bool isAvailableSpace;
    SDL_Surface *surf;
    bool visible;
};

#endif
