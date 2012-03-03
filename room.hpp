#ifndef ROOM_H
#define ROOM_H

#include <vector>
#include <SDL/SDL.h>
#include "map.hpp"
#include "geometry.hpp"
#include "enums.hpp"

class Map;
class Room;
class Tile;

typedef enum {
    NORTH,
    SOUTH,
    EAST,
    WEST,
    LAST_DIRECTION,
    NO_DIRECTION
} Direction;

class Room {
    public:
    Room(Map *m);
    Room(Map *m, Room *parent, Direction from_direction, SDL_Rect area);
    ~Room(void);
    void FindChild(void);
    SDL_Rect FindRoomSpace(Direction d, SDL_Rect *corridor);
    bool hasChildrenAvailable(void);
    bool hasPathInDirection(Direction d);
    SDL_Rect FindCorridor(Direction d);
    Direction ReverseDirection(Direction d);
    void Reveal(void);
    void RevealPath(Direction d);
    void RevealRoomInDirection(Direction d);
    Room* GetRoomInDirection(Direction d);
    void Travel(Direction d);
    void CentreCamera(void);
    void AddObject(Room_Object robj);
    Tile* FindSpaceForObject(void);
    int CountExits(void);

    Map *map;
    Room *children[LAST_DIRECTION];
    SDL_Rect corridors[LAST_DIRECTION];
    Room *parent;
    SDL_Rect space;
    std::vector<Room_Object> room_contents;

    private:
};

#endif
