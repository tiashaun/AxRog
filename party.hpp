#ifndef PARTY_H
#define PARTY_H

#include "character.hpp"
#include "room.hpp"

class Party {
    public:
    Party(void);
    ~Party(void);
    void SetLocation(Room *r);
    void Travel(Direction d);

    Character *characters[6];
    int gold;
    Room *location;
};

#endif
