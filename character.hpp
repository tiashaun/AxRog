#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL/SDL.h>
#include "entity.hpp"
#include "enums.hpp"
#include "item/weapon.hpp"
#include "item/helm.hpp"
#include "item/armour.hpp"
#include "item/misc.hpp"

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

    Helm                    *helm;
    Weapon                  *weapon;
    Armour                  *armour;
    Misc                    *misc1;
    Misc                    *misc2;
};

#endif
