#ifndef EQUIPPABLE_H
#define EQUIPPABLE_H

#include "item.hpp"
#include "enums.hpp"
#include <string>

namespace Equippable {
    struct EItem : Item {
        inline EItem(const std::string name, int value, Equipment::Type type,
            int att_mod, int def_mod, int str_mod, int tou_mod, int wil_mod,
            int spd_mod) : Item(name, value), att_mod(att_mod), def_mod(def_mod),
            str_mod(str_mod),tou_mod(tou_mod), wil_mod(wil_mod), spd_mod(spd_mod) {}
        Equipment::Type type;
        int att_mod;
        int def_mod;
        int str_mod;
        int tou_mod;
        int wil_mod;
        int spd_mod;
    };

    extern EItem equipment_list[];
}

#endif
