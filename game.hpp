#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>
#include "map.hpp"
#include "party.hpp"

class Game {
    public:
    
    Game(void);
    ~Game(void);
    bool isRunning(void);
    void Update(void);
    void DrawTo(SDL_Surface *surf);
    void FramePause(void);

    Map *map;
    Party *party;
    bool running;
    int last_update;
    int camera_x;
    int camera_y;
};

#endif
