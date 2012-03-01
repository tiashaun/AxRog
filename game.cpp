#include "game.hpp"

#define CAMERA_SPEED    150
#define FRAMERATE       20

Game::Game(SDL_Surface *screen) {
    map = new Map(500, 500, screen);
    party = new Party();

    framedelay = 1000/FRAMERATE;
    running = true;
    needRedraw = true;
}

Game::~Game(void) {
    delete map;
    delete party;
}

bool
Game::isRunning(void) {
    return running;
}

void
Game::Update(SDL_Surface *surf) {
    if (needRedraw) {
        DrawTo(surf);
        needRedraw = false;
    }
}

void
Game::DrawTo(SDL_Surface *surf) {
    map->DrawTo(surf);
}

void
Game::FramePause(void) {
    Uint32 t;

    t = SDL_GetTicks();
    if (t - lastupdate < framedelay)
        SDL_Delay(framedelay - (t - lastupdate));

    lastupdate = SDL_GetTicks();
}

void
Game::HandleInput(SDL_Event *e) {
    while ( SDL_PollEvent(e) ) {
        switch (e->type) {
            case SDL_KEYDOWN:
                HandleKeypress(&e->key);
                break;
            case SDL_KEYUP:
                break;
            case SDL_QUIT:
                running = false;
                break;
        }
    }
}

void
Game::HandleKeypress(SDL_KeyboardEvent *key) {
    switch(key->keysym.sym) {
        case SDLK_ESCAPE:
            running = 0;
            break;
        case SDLK_LEFT:
            map->MoveCamera(-CAMERA_SPEED, 0);
            needRedraw = true;
            break;
        case SDLK_RIGHT:
            map->MoveCamera(+CAMERA_SPEED, 0);
            needRedraw = true;
            break;
        case SDLK_UP:
            map->MoveCamera(0, -CAMERA_SPEED);
            needRedraw = true;
            break;
        case SDLK_DOWN:
            map->MoveCamera(0, CAMERA_SPEED);
            needRedraw = true;
            break;
        default:
            break;
    }
}
