#ifndef MAP_H
#define MAP_H

#include <SDL/SDL.h>
#include "room.hpp"
#include "tile.hpp"

class Room;
class Tile;

class Map {
    public:
    Map(int w, int h, SDL_Surface *screen);
    ~Map(void);
    Tile* GetTile(int x, int y);
    void DrawTo(SDL_Surface *surf);
    void SetCamera(int x, int y, int w, int h);
    void MoveCamera(int x, int y);
    void ApplyRoom(Room *r);
    void ApplyCorridor(SDL_Rect r);
    void FloorRect(SDL_Rect *r);
    void VisibleRect(SDL_Rect *r);
    bool isSpaceAvailable(SDL_Rect *r);
    void CameraClip(void);
    void CameraToRect(SDL_Rect *r);

    Room *rooms;
    int width;
    int height;
    SDL_Rect cam;
    Tile *tiles;

    private:
};

#endif
