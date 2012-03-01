#include "map.hpp"
#include <cmath>
#include <cstdlib>

#define TILE_SZ     32

Map::Map(int w, int h, SDL_Surface *screen) {
    int i;

    width = w;
    height = h;
    cam.x = 0;
    cam.y = 0;
    cam.w = screen->w;
    cam.h = screen->h;

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
    SDL_Rect from;
    Tile *t;
    int first_x, last_x;
    int first_y, last_y;

    // TODO: Change to black
    SDL_FillRect(surf, NULL, 376784);

    // Ceil and floor swapped wile I work a way to do this without overlappign edges
    first_x = (cam.x == 0) ? 0 : ceil((float)cam.x / TILE_SZ);
    last_x = floor((float) (cam.x + surf->w - 10) / TILE_SZ);
    first_y = (cam.y == 0) ? 0 : ceil((float)cam.y / TILE_SZ);
    last_y = floor((float) (cam.y + surf->h - 10) / TILE_SZ);

    to.x = 0;
    to.y = 0;
    to.w = to.h = TILE_SZ;

    from.x = 0;
    from.y = 0;
    from.w = from.h = TILE_SZ;

    for (int e = first_y; e < last_y; ++e) {
        to.y = e * TILE_SZ - cam.y;
        for (int i = first_x; i < last_x; ++i) {
            to.x = i * TILE_SZ - cam.x;

            t = GetTile(i, e);
            if(!t)
                continue;
            if(!t->visible)
                continue;
            SDL_BlitSurface( t->surf, &from, surf, &to );

        }
    }
}

void
Map::MoveCamera(int x, int y) {
    cam.x += x;
    cam.y += y;

    if (cam.x < 0)
        cam.x = 0;
    if (cam.y < 0)
        cam.y = 0;
    if (cam.x + cam.w > width * TILE_SZ)
        cam.x = width * TILE_SZ - cam.w;
    if (cam.y + cam.h > height * TILE_SZ)
        cam.y = height * TILE_SZ - cam.h;
}
