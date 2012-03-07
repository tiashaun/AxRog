#ifndef ITEM_H
#define ITEM_H

#include <string>

struct Item {
    inline Item(const std::string name, int value) : name(name), value(value) {}
    std::string name;
    int value;
};

#endif
