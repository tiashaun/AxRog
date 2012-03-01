#include "room.hpp"

#define CHILD_TRIES     20

Room::Room(Map *m) {
    map = m;
    north = NULL;
    south = NULL;
    east = NULL;
    west = NULL;

    FindChild();
}

Room::Room(Map *m, Room *n, Room *s, Room *e, Room *w) {
    map = m;
    north = n;
    south = s;
    east = e;
    west = w;
}

Room::~Room(void) {
    if (north)
        delete north;
    if (south)
        delete south;
    if (east)
        delete east;
    if (west)
        delete west;
}

Room*
Room::FindChild(void) {
    return NULL;
}
