#include <SDL/SDL.h>
#include "game.hpp"

int main(int argc, char **argv) {
    Game *game;
    SDL_Surface *screen;

    SDL_Init(SDL_INIT_EVERYTHING);
    screen = SDL_SetVideoMode( 800, 600, 32, SDL_HWSURFACE );

    game = new Game();
    game->DrawTo(screen);
    while ( game->isRunning() ) {
        game->Update();
        game->FramePause();
        SDL_Flip(screen);
    }

    delete game;
    SDL_FreeSurface(screen);
    SDL_Quit();

    return 0;
}
