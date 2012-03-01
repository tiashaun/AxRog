#include "game.hpp"

Game::Game(void) {
    map = new Map(40, 40);
    party = new Party();
    running = true;
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
Game::drawTo(SDL_Surface *screen) {

}

void
Game::framePause(void) {

}
