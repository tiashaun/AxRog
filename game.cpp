#include "game.hpp"

#define CAMERA_SPEED    150
#define FRAMERATE       20

Game::Game(SDL_Surface *screen) {
    this->map = new Map(60, 60, screen);
    this->party = new Party();

    this->party->SetLocation(map->rooms);

    this->framedelay = 1000/FRAMERATE;
    this->running = true;
    this->needRedraw = true;
    this->lastupdate = 0;
    this->mode = GameMode::MOVEMENT;
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
    switch (this->mode) {
        case GameMode::MOVEMENT :
            map->DrawTo(surf);
            map->DrawPartyMarker(surf, party->location);
            break;
        case GameMode::CHAR_SCREEN :
            this->DrawCharacterScreen(surf);
            break;
        default:
            break;
    }
}

void
Game::DrawCharacterScreen(SDL_Surface *surf) {
    SDL_Rect dest;

    dest.x = (surf->w - 1024) / 2;
    dest.y = (surf->h - 600) / 2;
    dest.w = 1024;
    dest.h = 600;

    SDL_FillRect(surf, &dest, 324423);
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
                HandleKeypressAll(&e->key);
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
Game::HandleKeypressAll(SDL_KeyboardEvent *key) {
    if (this->mode == GameMode::MOVEMENT)
        HandleKeypressMovement(key);
    else if (this->mode == GameMode::CHAR_SCREEN)
        HandleKeypressCharScreen(key);
}

void
Game::HandleKeypressCharScreen(SDL_KeyboardEvent *key) {
    switch(key->keysym.sym) {
        case SDLK_ESCAPE :
            this->mode = GameMode::MOVEMENT;
            needRedraw = true;
            return;
        default:
            break;
    }
}

void
Game::HandleKeypressMovement(SDL_KeyboardEvent *key) {
    switch(key->keysym.sym) {
        case SDLK_ESCAPE:
            running = 0;
            break;
        case SDLK_w:
            map->MoveCamera(0, -CAMERA_SPEED);
            needRedraw = true;
            break;
        case SDLK_a:
            map->MoveCamera(-CAMERA_SPEED, 0);
            needRedraw = true;
            break;
        case SDLK_s:
            map->MoveCamera(0, CAMERA_SPEED);
            needRedraw = true;
            break;
        case SDLK_d:
            map->MoveCamera(+CAMERA_SPEED, 0);
            needRedraw = true;
            break;
        case SDLK_UP:
            party->Travel(NORTH);
            needRedraw = true;
            break;
        case SDLK_DOWN:
            party->Travel(SOUTH);
            needRedraw = true;
            break;
        case SDLK_LEFT:
            party->Travel(WEST);
            needRedraw = true;
            break;
        case SDLK_RIGHT:
            party->Travel(EAST);
            needRedraw = true;
            break;
        case SDLK_c:
            this->UpdateCharScreen();
            this->mode = GameMode::CHAR_SCREEN;
            needRedraw = true;
            break;
        default:
            break;
    }
}

void
Game::UpdateCharScreen(void) {
    
}
