#ifndef ROOMOBJECT_H
#define ROOMOBJECT_H

#include <SDL/SDL.h>

typedef enum {
    STAIRS_UP,
    STAIRS_DOWN,
} Room_Object_Type;

struct Point {
    int x;
    int y;
};

class RoomObject {
    SDL_Surface *surf;
    Point loc;  
};

#endif
