#include <vector>
#include "room.hpp"

#define CHILD_TRIES     20

Room::Room(Map *m) {
    map = m;
    children[NORTH] = NULL;
    children[SOUTH] = NULL;
    children[EAST] = NULL;
    children[WEST] = NULL;

    space = FindRoomSpace(NO_DIRECTION, NULL);

    map->ApplyRoom(this);

    for (int i = 0; i < CHILD_TRIES; ++i) {
        if (!hasChildrenAvailable())
            break;
        FindChild();
    }

}

Room::Room(Map *m, Room *n, Room *s, Room *e, Room *w) {
    map = m;
    children[NORTH] = n;
    children[SOUTH] = s;
    children[EAST] = e;
    children[WEST] = w;
}

Room::~Room(void) {
    for (int i = 0; i < LAST_DIRECTION; ++i)
        if (children[i])
            delete children[i];
}

void
Room::FindChild(void) {
    std::vector<Direction> available;
    SDL_Rect corridor;
    SDL_Rect roomspace;
    int distance;
    Direction d;

    for (int i = NORTH; i < LAST_DIRECTION; ++i) {
        if (!children[i])
            available.push_back((Direction) i);
    }

    d = available[rand() % available.size()];

    //At this point we have one randomly selected available direction
    //And we need to make our corridor
    corridor = FindCorridor(d);
    if( !map->isSpaceAvailable(&corridor) )
        return;

    roomspace = FindRoomSpace(d, &corridor);
    if (roomspace.w == 0 || roomspace.h == 0)
        return;
    // TODO: Find room space to attach to corridor
    // if room space is free then apply the corridor
    // and add the new room as a child
    // the new room will then try to add children
}

SDL_Rect
Room::FindRoomSpace(Direction d, SDL_Rect *corridor) {
    SDL_Rect ret;

    ret.w = rand() % 5 + 3;
    ret.h = rand() % 5 + 3;
    if (d == NO_DIRECTION) {
        ret.x = rand() % (map->width - ret.w - 2) + 1;
        ret.y = rand() % (map->height - ret.h - 2) + 1;
    }
    else
        return (SDL_Rect){0, 0, 0, 0};

    if ( !map->isSpaceAvailable(&ret) )
        return (SDL_Rect){0, 0, 0, 0};

    return ret;
}

SDL_Rect
Room::FindCorridor(Direction d) {
    SDL_Rect ret;

    if (d == NORTH || d == SOUTH) {
        ret.w = 1;
        ret.h = rand() % 5 + 3;
    }
    else { //d == EAST || d == WEST
        ret.h = 1;
        ret.w = rand() % 5 + 3;
    }

    if (d == NORTH) {
        ret.x = space.x + rand() % space.w;
        ret.y = space.y - ret.h;
    }
    else if (d == SOUTH) {
        ret.x = space.x + rand() % space.w;
        ret.y = space.y + space.h;
    }
    else if (d == EAST) {
        ret.x = space.x + space.w;
        ret.y = space.y + rand() % space.h;
    }
    else { //if d == WEST
        ret.x = space.x - ret.w;
        ret.y = space.y + rand() % space.h;
    }

    return ret;
}

bool
Room::hasChildrenAvailable(void) {
    return !(children[NORTH] && children[SOUTH] && children[EAST] && 
        children[WEST]);
}
