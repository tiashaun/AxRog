#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL/SDL.h>
#include "entity.hpp"
#include "enums.hpp"

class Character : public Entity {
    public:
    Character(Species::Type inSpecies, CharacterClass::Type inClass);
    void DrawPartyScreenLine(SDL_Surface *surf, SDL_Rect *dest);

    int                     level;
    int                     XP;
    CharacterClass::Type    cclass;
    Species::Type           species;
    SDL_Surface             *portrait;
};

#endif
