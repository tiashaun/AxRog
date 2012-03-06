#ifndef FONTHANDLER_H
#define FONTHANDLER_H

#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

namespace FontHandler {
    void Init(std::string path);
    void WriteText(SDL_Surface *surf, SDL_Rect pos, std::string msg);

    static TTF_Font     *font;
    static SDL_Color    fontcolor;
};

#endif
