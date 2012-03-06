#ifndef ITEM_H
#define ITEM_H

#include <string>

class Item {
    public:
    Item(char *name) {
        this->name = name;
    }

    int value;
    std::string name;
};

#endif
