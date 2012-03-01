#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>
#include "map.hpp"
#include "party.hpp"

class Game {
    public:
    
    Game(SDL_Surface *screen);
    ~Game(void);
    bool isRunning(void);
    void Update(SDL_Surface *surf);
    void DrawTo(SDL_Surface *surf);
    void FramePause(void);
    void HandleInput(SDL_Event *e);
    void HandleKeypress(SDL_KeyboardEvent *key);

    Map *map;
    Party *party;
    bool running;
    bool needRedraw;
    int framedelay;
    int lastupdate;
};

#endif
