#ifndef ENUMS_H
#define ENUMS_H

namespace GameMode {
    typedef enum {
        MOVEMENT,
        CHAR_SCREEN,
    } Type;
};

namespace RoomObject {
    typedef enum {
        STAIRS_UP,
        STAIRS_DOWN
    } Type;
};

namespace CharacterClass {
    typedef enum {
        FIGHTER,
        ROGUE,
        CLERIC,
        WIZARD,
        LAST_CLASS,
    } Type;
};

namespace Species {
    typedef enum {
        OTHER,
        HUMAN,
        ELF,
        HALF_ELF,
        ORC,
        LAST_RACE
    } Type;
};

#endif
