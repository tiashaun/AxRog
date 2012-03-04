#include "map.hpp"
#include <cmath>
#include <cstdlib>

#define MAX(x, y) ((x > y) ? (x) : (y))
#define MIN(x, y) ((x < y) ? (x) : (y))

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
        tiles[i].MakeWall();
    }

    lastroom = NULL;
    rooms = new Room(this);
    //This next bit is a bad case but lets be safe...
    if (this->lastroom == NULL) {
        lastroom = rooms;
        rooms->AddObject(RoomObject::STAIRS_DOWN);
    }

    CameraToRect(&rooms->space);
}

Map::~Map(void) {
    delete [] tiles;
    delete rooms;
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
Map::ApplyRoom(Room *r) {
    SDL_Rect rect;
    Tile *t;

    rect = r->space;
    FloorRect(&rect);
}

void
Map::ApplyCorridor(SDL_Rect r) {
    SDL_Rect door1, door2;
    door1.x = r.x;
    door1.y = r.y;
    if (r.w == 1) { // Corridor is NORTH-SOUTH
        door2.x = r.x;
        door2.y = r.y + r.h - 1;
        //resize for floor-making
        r.y += 1;
        r.h -= 2;
    }
    else { //Corridor is EAST-WEST
        door2.x = r.x + r.w - 1;
        door2.y = r.y;
        //resize for floor-making
        r.x += 1;
        r.w -= 2;
    }
    GetTile(door1.x, door1.y)->MakeDoor();
    GetTile(door2.x, door2.y)->MakeDoor();
    FloorRect(&r);
}

void
Map::FloorRect(SDL_Rect *r) {
    Tile *t;

     for (int e = r->y; e < r->y + r->h; ++e) {
        for (int i = r->x; i < r->x + r->w; ++i) {
            t = GetTile(i, e);
            t->MakeFloor();
        }
    }
}

void
Map::VisibleRect(SDL_Rect *r) {
    int e, i;
    Tile *t;

    if (r->h == 0 || r->w == 0)
        return;

    for (e = MAX(r->y - 1, 0); MIN(e < r->y + r->h + 1, width); ++e) {
        for (i = MAX(r->x - 1, 0); MIN(i < r->x + r->w + 1, width); ++i) {
            t = GetTile(i, e);
            t->visible = true;
        }
    }
}

void
Map::DrawTo(SDL_Surface *surf) {
    int first_x, last_x;
    int first_y, last_y;
    Tile *t;
    SDL_Rect src;
    SDL_Rect dest;

    SDL_FillRect(surf, NULL, 0);

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
            if (!t)
                continue;
            if (!t->visible)
                continue;
            SDL_BlitSurface( t->surf, &src, surf, &dest );
        }
    }
}

void
Map::MoveCamera(int x, int y) {
    cam.x += x;
    cam.y += y;

    CameraClip();
}

bool
Map::isSpaceAvailable(SDL_Rect *r) {
    Tile *t;

    if (r->x <= 0 || r->y <= 0)
        return false;
    if (r->x + r->w >= width || r->y + r->h >= height)
        return false;

    for(int e = r->y; e < r->y + r->h; ++e) {
        for(int i = r->x; i < r->x + r->w; ++i) {
            t = GetTile(i, e);
            if ( !t->isAvailableSpace )
                return false;
        }
    }

    return true;
}

void
Map::CameraClip(void) {
    if (cam.x < 0)
        cam.x = 0;
    if (cam.y < 0)
        cam.y = 0;
    if (cam.x + cam.w > width * TILE_SZ)
        cam.x = width * TILE_SZ - cam.w;
    if (cam.y + cam.h > height * TILE_SZ)
        cam.y = height * TILE_SZ - cam.h;
}

void
Map::CameraToRect(SDL_Rect *r) {
    cam.x = ((r->x + r->w / 2) * TILE_SZ) - cam.w / 2;
    cam.y = ((r->y + r->h / 2) * TILE_SZ) - cam.h / 2;
    
    CameraClip();
}
