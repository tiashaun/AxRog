#ifndef TILE_H
#define TILE_H

#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <string>
#include "resource.hpp"

class Tile {
    public:
    void load(std::string str);

    SDL_Surface *surf;
};

#endif
