#ifndef ROOM_H
#define ROOM_H

#include <SDL/SDL.h>
#include "map.hpp"

class Map;
class Room;

class Room {
    public:
    Room(Map *m);
    Room(Map *m, Room *n, Room *s, Room *e, Room *w);
    ~Room(void);
    Room* FindChild(void);
    void ApplyToMap(void);

    Map *map;
    Room *north;
    Room *south;
    Room *east;
    Room *west;
    SDL_Rect space;

    private:
};

#endif
