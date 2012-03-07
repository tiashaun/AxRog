#ifndef EQUIPPABLE_H
#define EQUIPPABLE_H

#include "item.hpp"

class Equippable : public Item {
    public:
    int att_mod;
    int def_mod;
    int str_mod;
    int tou_mod;
    int wil_mod;
    int spd_mod;
};

class Weapon : public Equippable {};
class Armour : public Equippable {};
class HeadGear : public Equippable {};
class Shield : public Equippable {};
class Boots : public Equippable {};
class Misc : public Equippable {};

#endif
