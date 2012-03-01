#ifndef MAP_H
#define MAP_H

#include <SDL/SDL.h>
#include "room.hpp"
#include "tile.hpp"

class Map {
    public:
    Map(int w, int h, SDL_Surface *screen);
    ~Map(void);
    Tile* GetTile(int x, int y);
    void DrawTo(SDL_Surface *surf);
    void SetCamera(int x, int y, int w, int h);
    void MoveCamera(int x, int y);

    Room *start_room;
    Room *end_room;
    int width;
    int height;
    SDL_Rect cam;
    Tile *tiles;

    private:
};

#endif
