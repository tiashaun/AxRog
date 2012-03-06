#ifndef PARTY_H
#define PARTY_H

#define MAX_PARTY_SIZE  6

#include "character.hpp"
#include "room.hpp"

class Party {
    public:
    Party(void);
    ~Party(void);
    void SetLocation(Room *r);
    void Travel(Direction d);

    Character *characters[MAX_PARTY_SIZE];
    int gold;
    Room *location;
    SDL_Surface* loc_indicator;
};

#endif
