#include <SDL/SDL.h>
#include "game.hpp"
#include "resource.hpp"
#include "fonthandler.hpp"

static int SCREEN_WIDTH;
static int SCREEN_HEIGHT;

static void
SetDefaults(void) {
    SCREEN_WIDTH = 1024;
    SCREEN_HEIGHT = 600;
}

static void
HandleArgs(int argc, char **argv) {
    char c;

    while( (c = getopt(argc, argv, "w:h:x:y:")) != -1 ) {
        switch (c) {
            case 'w':
            case 'x':
                SCREEN_WIDTH = atoi(optarg);
                break;
            case 'h':
            case 'y':
                SCREEN_HEIGHT = atoi(optarg);
                break;
            case '?':
            default:
                break;
        }
    }
}

int main(int argc, char **argv) {
    Game *game;
    SDL_Surface *screen;
    SDL_Event events;

    srand(time(NULL));
    SetDefaults();
    HandleArgs(argc, argv);

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_EnableKeyRepeat(300, 50);
    SDL_WM_SetCaption("Ax Rogue", NULL);
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE );

    RSM::Init();
    FontHandler::Init(std::string("res/Font.ttf"));

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
