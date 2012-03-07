#ifndef CHARACTER_H
#define CHARACTER_H

#include <SDL/SDL.h>
#include "entity.hpp"
#include "enums.hpp"
#include "equippable.hpp"

class Character : public Entity {
    public:
    Character(std::string name, Species::Type inSpecies,
        CharacterClass::Type inClass);
    ~Character(void);
    void DrawPartyScreenLine(SDL_Surface *surf, SDL_Rect dest);
    void TotalGearModifiers(void);

    int                     level;
    int                     XP;
    CharacterClass::Type    cclass;
    Species::Type           species;
    SDL_Surface             *portrait;

    int                     tot_att;
    int                     tot_def;
    int                     tot_str;
    int                     tot_tou;
    int                     tot_wil;
    int                     tot_spd;

    HeadGear                *headgear;
    Armour                  *armour;
    Item                    *shield;
    Weapon                  *weapon;
    Boots                   *boots;
    Misc                    *misc;
};

#endif
