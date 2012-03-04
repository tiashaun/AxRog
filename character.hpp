#ifndef CHARACTER_H
#define CHARACTER_H

#include "entity.hpp"

class Character : public Entity {
    public:
    int level;
    int XP;
};

#endif
