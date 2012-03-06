#ifndef ENTITY_H
#define ENTITY_H

#include <string>

class Entity {
    public:
    Entity(void);

    std::string name;
    int max_hp;
    int curr_hp;
    int max_mp;
    int curr_mp;
    int att;
    int def;
    int str;
    int tou;
    int wil;
    int spd;
};

#endif
