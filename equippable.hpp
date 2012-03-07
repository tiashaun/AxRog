#ifndef EQUIPPABLE_H
#define EQUIPPABLE_H

#include "item.hpp"
#include "enums.hpp"

class Equippable : public Item {
    public:
    Equippable(char *name, Equipment::Type, int att, int def, int str, int tou,
        int wil, int spd);

    Equipment::Type type;
    int att_mod;
    int def_mod;
    int str_mod;
    int tou_mod;
    int wil_mod;
    int spd_mod;
};

#endif
