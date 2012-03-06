#include "fonthandler.hpp"
#include "resource.hpp"

void
FontHandler::Init(std::string path) {
    TTF_Init();
    font = TTF_OpenFont( path.c_str(), 14 );
    fontcolor.r = 0;
    fontcolor.g = 0;
    fontcolor.b = 0;
}

void
FontHandler::WriteText(SDL_Surface *surf, SDL_Rect pos, std::string msg) {
    SDL_Surface *message;
    message = TTF_RenderText_Blended( font, msg.c_str(), fontcolor );
    SDL_BlitSurface(message, NULL, surf, &pos);
    SDL_FreeSurface(message);
}
