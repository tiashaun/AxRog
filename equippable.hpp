#ifndef EQUIPPABLE_H
#define EQUIPPABLE_H

#include "item.hpp"

class Equippable : public Item {

};

class Weapon : public Equippable {};
class Armour : public Equippable {};
class Helm : public Equippable {};
class Shield : public Equippable {};
class Boots : public Equippable {};
class Misc : public Equippable {};

#endif
