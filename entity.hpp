#ifndef ENTITY_H
#define ENTITY_H

#include <string>

class Entity {
    public:
    Entity(void);

    std::string name;
    int max_health;
    int curr_health;
    int att;
    int def;
    int str;
    int tou;
    int wil;
    int spd;
};

#endif
