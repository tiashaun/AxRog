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
    void DrawCharacterScreen(SDL_Surface *surf);
    void DrawCharacterScreenLine(SDL_Surface *surf, SDL_Rect *dest, Character *c);
    void FramePause(void);
    void HandleInput(SDL_Event *e);
    void HandleKeypressAll(SDL_KeyboardEvent *key);
    void HandleKeypressCharScreen(SDL_KeyboardEvent *key);
    void HandleKeypressMovement(SDL_KeyboardEvent *key);
    void UpdateCharScreen(void);

    GameMode::Type  mode;
    Map             *map;
    Party           *party;
    bool            running;
    bool            needRedraw;
    int             framedelay;
    int             lastupdate;
};

#endif
