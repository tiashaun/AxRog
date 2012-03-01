#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>
#include "map.hpp"
#include "party.hpp"

class Game {
    public:
    
    Game(void);
    ~Game(void);
    void run(void);
    bool isRunning(void);
    void drawTo(SDL_Surface *screen);
    void framePause(void);

    Map *map;
    Party *party;
    bool running;
    int last_update;
};

#endif
