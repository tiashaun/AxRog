#ifndef TILE_H
#define TILE_H

#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <string>
#include "room.hpp"
#include "resource.hpp"
#include "enums.hpp"

class Tile {
    public:
    Tile(void);
    ~Tile(void);
    void MakeWall(void);
    void MakeFloor(void);
    void MakeDoor(void);
    void AddObject(Room_Object robj);

    bool isAvailableSpace;
    SDL_Surface *surf;
    SDL_Surface *object;
    bool visible;
};

#endif
