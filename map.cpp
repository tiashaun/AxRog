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
    int first_x, last_x;
    int first_y, last_y;
    Tile *t;
    SDL_Rect src;
    SDL_Rect dest;

    SDL_FillRect(surf, NULL, 376784);

    first_x = (cam.x == 0) ? 0 : floor((float) cam.x / TILE_SZ);
    last_x = ceil((float)(cam.x + cam.w) / TILE_SZ);
    first_y = (cam.y == 0) ? 0 : floor((float) cam.y / TILE_SZ);
    last_y = ceil((float)(cam.y + cam.h) / TILE_SZ);

    for (int e = first_y; e < last_y; ++e) {
        src.y = 0;
        src.h = TILE_SZ;
        dest.h = TILE_SZ;

        dest.y = e * TILE_SZ - cam.y;
        if(dest.y < 0) {
            src.y = -dest.y;
            src.h -= src.y;
            dest.y = 0;
        }
        else if (dest.y + dest.h > cam.h) {
            dest.h -= dest.y + dest.h - cam.h;
            src.h = dest.h;
        }

        for (int i = first_x; i < last_x; ++i) {
            src.x = 0;
            src.w = TILE_SZ;
            dest.w = TILE_SZ;

            dest.x = i * TILE_SZ - cam.x;
            if(dest.x < 0) {
                src.x = -dest.x;
                src.w -= src.x;
                dest.x = 0;
            }
            else if (dest.x + dest.w > cam.w) {
                dest.w -= dest.x + dest.w - cam.w;
                src.w = dest.w;
            }

            t = GetTile(i, e);
            SDL_BlitSurface( t->surf, &src, surf, &dest );
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
