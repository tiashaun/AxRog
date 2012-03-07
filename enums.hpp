#ifndef ENUMS_H
#define ENUMS_H

namespace Equipment {
    typedef enum {
        HEADGEAR,
        WEAPON,
        ARMOUR,
        SHIELD,
        BOOTS,
        MISC,
        LAST_EQUIPMENT_TYPE,
    } Type;
};

namespace GameMode {
    typedef enum {
        MOVEMENT,
        PARTY_SCREEN,
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
        PALADIN,
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
