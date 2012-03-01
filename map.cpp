#include "map.hpp"
#include <cstdlib>

#define TILE_SZ     50

Map::Map(int w, int h) {
    int i;

    width = w;
    height = h;
    cam_x = 0;
    cam_y = 0;

    tiles = new Tile[h*w];

    for (i = 0; i < h * w; ++i) {
        tiles[i].load("res/tiles/floor.png");
    }
}

Map::~Map(void) {
    delete tiles;
}

Tile*
Map::GetTile(int x, int y) {
    int index;

    if (x < 0 || x > width)
        return NULL;
    if (y < 0 || y > height)
        return NULL;
    index = y * width + x;

    return &tiles[index];
}

void
Map::DrawTo(SDL_Surface *surf) {
    SDL_Rect to;

    to.w = to.h = TILE_SZ;

    for (int e = 0; e < 10; ++e) {
        to.y = e * TILE_SZ;
        for (int i = 0; i < 10; ++i) {
            to.x = i * TILE_SZ;
            SDL_BlitSurface( GetTile(i, e)->surf, NULL, surf, &to );
        }
    }
}
