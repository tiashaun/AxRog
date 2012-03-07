#include "equippable.hpp"

Equippable::Equippable(char *name, Equipment::Type type, int att, int def,
        int str, int tou, int wil, int spd) : Item(name) {
    this->type = type;
    this->att_mod = att;
    this->def_mod = def;
    this->str_mod = str;
    this->tou_mod = tou;
    this->wil_mod = wil;
    this->spd_mod = spd;
}
