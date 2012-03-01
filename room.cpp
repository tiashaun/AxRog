#include "room.hpp"

#define CHILD_TRIES     20

Room::Room(Map *m) {
    map = m;
    north = NULL;
    south = NULL;
    east = NULL;
    west = NULL;

    space.w = 4;
    space.h = 5;
    space.x = 15;
    space.y = 15;

    FindChild();

    ApplyToMap();
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

void
Room::ApplyToMap(void) {
    Tile *t;
    for (int e = space.y; e < space.y + space.h; ++e) {
        for (int i = space.x; i < space.x + space.w; ++i) {
            t = map->GetTile(i, e);
            t->load("res/tiles/floor.png");
        }
    }

    for (int e = space.y - 1; e < space.y + space.h + 1; ++e) {
        for (int i = space.x - 1; i < space.x + space.w + 1; ++i) {
            t = map->GetTile(i, e);
            t->visible = true;
        }
    }

}
