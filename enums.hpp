#ifndef ENUMS_H
#define ENUMS_H

namespace RoomObject {
    typedef enum {
        STAIRS_UP,
        STAIRS_DOWN
    } Type;
};

namespace CharacterClass {
    typedef enum {
        FIGHTER,
        WIZARD,
        CLERIC,
        ROGUE,
        LAST_CLASS,
    } Type;
};

#endif
