#include <vector>
#include "room.hpp"

#define CHILD_TRIES     20

Room::Room(Map *m) {
    map = m;
    children[NORTH] = NULL;
    children[SOUTH] = NULL;
    children[EAST] = NULL;
    children[WEST] = NULL;
    parent = NULL;

    space = FindRoomSpace(NO_DIRECTION, NULL);

    map->ApplyRoom(this);
    for (int i = 0; i < CHILD_TRIES; ++i) {
        if (!hasChildrenAvailable())
            break;
        FindChild();
    }

}

Room::Room(Map *m, Room *par, Direction from_direction, SDL_Rect area) {
    map = m;
    space = area;
    parent = par;

    for (int i = NORTH; i < LAST_DIRECTION; ++i) {
        if (i == from_direction)
            children[from_direction] = parent;
        else
            children[i] = NULL;

    }

    map->ApplyRoom(this);
    for (int i = 0; i < CHILD_TRIES; ++i) {
        if (!hasChildrenAvailable())
            break;
        FindChild();
    }
}

Room::~Room(void) {
    for (int i = 0; i < LAST_DIRECTION; ++i) {
        if (children[i]) {
            if (parent && children[i] != parent) {
                delete children[i];
            }
        }
    }
}

void
Room::FindChild(void) {
    std::vector<Direction> available;
    SDL_Rect corridor;
    SDL_Rect roomspace;
    SDL_Rect roomspace_plusbuffer;
    int distance;
    Direction d;

    for (int i = NORTH; i < LAST_DIRECTION; ++i) {
        if (!children[i])
            available.push_back((Direction) i);
    }

    d = available[rand() % available.size()];

    // DEBUG
    // d = NORTH;

    //At this point we have one randomly selected available direction
    //And we need to make our corridor
    corridor = FindCorridor(d);
    if( !map->isSpaceAvailable(&corridor) )
        return;

    roomspace = FindRoomSpace(d, &corridor);
    if (roomspace.w == 0 || roomspace.h == 0)
        return;

    roomspace_plusbuffer.x = roomspace.x - 3;
    roomspace_plusbuffer.y = roomspace.y - 3;
    roomspace_plusbuffer.w = roomspace.w + 6;
    roomspace_plusbuffer.h = roomspace.h + 6;
    if( !map->isSpaceAvailable(&roomspace_plusbuffer) )
        return;

    map->ApplyCorridor(corridor);
    children[d] = new Room(map, this, ReverseDirection(d), roomspace);
}

SDL_Rect
Room::FindRoomSpace(Direction d, SDL_Rect *corridor) {
    SDL_Rect ret;

    ret.w = rand() % 5 + 3;
    ret.h = rand() % 5 + 3;
    if (d == NORTH) {
        ret.x = corridor->x - rand() % ret.w;
        ret.y = corridor->y - ret.h;
    }
    else if (d == SOUTH) {
        ret.x = corridor->x - rand() % ret.w;
        ret.y = corridor->y + corridor->h;
    }
    else if (d == EAST) {
        ret.x = corridor->x + corridor->w;
        ret.y = corridor->y - rand() % ret.h;
    }
    else if (d == WEST) {
        ret.x = corridor->x - ret.w;
        ret.y = corridor->y - rand() % ret.h;
    }
    else { //d = NO_DIRECTION
        ret.x = rand() % (map->width - ret.w - 2) + 1;
        ret.y = rand() % (map->height - ret.h - 2) + 1;
    }

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

Direction
Room::ReverseDirection(Direction d) {
    switch (d) {
        case NORTH:
            return SOUTH;
        case SOUTH:
            return NORTH;
        case EAST:
            return WEST;
        default:
            return EAST;
    }
}
