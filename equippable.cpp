#include "equippable.hpp"

Equippable equipment_list[] = {

//         Name,            value, type,                  att,    def,    str,    tou,    wil,    spd
Equippable("Longsword",     50,    Equipment::WEAPON,     5,      0,      5,      0,      0,      0),
Equippable("Shortsword",    30,    Equipment::WEAPON,     5,      0,      3,      0,      0,      1),
Equippable("Dagger",        15,    Equipment::WEAPON,     7,      0,      2,      0,      0,      2),
Equippable("Mace",          20,    Equipment::WEAPON,     3,      0,      3,      0,      0,      0),

Equippable("Chainmail",     80,    Equipment::ARMOUR,     0,      -2,     0,      5,      0,      -1),
Equippable("Leather",       25,    Equipment::ARMOUR,     0,      -1,     0,      3,      0,      0),
Equippable("Robes",         5,     Equipment::ARMOUR,     0,      0,      0,      1,      0,      0),

};
