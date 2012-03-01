#include <ctime>
#include <SDL/SDL.h>
#include "game.hpp"
#include "resource.hpp"

int main(int argc, char **argv) {
    Game *game;
    SDL_Surface *screen;
    SDL_Event events;

    srand(time(NULL));

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_EnableKeyRepeat(300, 50);
    SDL_WM_SetCaption("Ax Rogue", NULL);
    screen = SDL_SetVideoMode( 1280, 1024, 32, SDL_HWSURFACE );

    game = new Game(screen);
    while ( game->isRunning() ) {
        game->HandleInput(&events);
        game->Update(screen);
        game->FramePause();
        SDL_Flip(screen);
    }

    delete game;
    RSM::CleanUp();
    SDL_FreeSurface(screen);
    SDL_Quit();

    return 0;
}
