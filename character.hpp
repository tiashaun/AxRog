#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL/SDL.h>
#include "entity.hpp"
#include "enums.hpp"

class Character : public Entity {
    public:

    Character(void);

    int level;
    int XP;
    CharacterClass::Type cclass;
    SDL_Surface *portrait;
    SDL_Surface *combat_back;
};

#endif
