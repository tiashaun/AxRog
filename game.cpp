#include "game.hpp"

Game::Game(void) {
    map = new Map(400, 400);
    party = new Party();

    running = true;
    camera_x = 0;
    camera_y = 0;
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
Game::Update(void) {

}

void
Game::DrawTo(SDL_Surface *surf) {
    map->DrawTo(surf);
}

void
Game::FramePause(void) {

}
