#include "equippable.hpp"

#define LENGTH(x) (sizeof(x) / sizeof(x[0]))

Equippable::EItem Equippable::eitem_list[] = {

//    Name,            value, type,                  att,    def,    str,    tou,    wil,    spd
EItem("Longsword",     50,    Equipment::WEAPON,     5,      0,      5,      0,      0,      0),
EItem("Shortsword",    30,    Equipment::WEAPON,     5,      0,      3,      0,      0,      1),
EItem("Dagger",        15,    Equipment::WEAPON,     7,      0,      2,      0,      0,      2),
EItem("Mace",          20,    Equipment::WEAPON,     3,      0,      3,      0,      0,      0),

EItem("Chainmail",     80,    Equipment::ARMOUR,     0,      -2,     0,      5,      0,      -1),
EItem("Leather Armour",25,    Equipment::ARMOUR,     0,      -1,     0,      3,      0,      0),
EItem("Robes",         5,     Equipment::ARMOUR,     0,      0,      0,      1,      0,      0),

};

Equippable::EItem*
Equippable::GetItem(std::string findstr) {
    for (int i = 0; i < LENGTH(eitem_list); ++i) {
        if (eitem_list[i].name == findstr)
            return &eitem_list[i];
    }
    return NULL;
}
