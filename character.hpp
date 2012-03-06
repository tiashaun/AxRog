#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL/SDL.h>
#include "entity.hpp"
#include "enums.hpp"
#include "weapon.hpp"
#include "helm.hpp"
#include "armour.hpp"
#include "misc.hpp"
#include "boots.hpp"

class Character : public Entity {
    public:
    Character(std::string name, Species::Type inSpecies,
        CharacterClass::Type inClass);
    ~Character(void);
    void DrawPartyScreenLine(SDL_Surface *surf, SDL_Rect dest);

    int                     level;
    int                     XP;
    CharacterClass::Type    cclass;
    Species::Type           species;
    SDL_Surface             *portrait;

    Helm                    *head;
    Armour                  *body;
    Item                    *left;
    Item                    *right;
    Boots                   *feet;
    Misc                    *misc;
};

#endif
