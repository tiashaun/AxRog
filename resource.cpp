#include "resource.hpp"
#include <iostream>
#include <SDL/SDL_image.h>

SDL_Surface*
RSM::GetSurface(std::string path) {
    Resource r;
    SDL_Surface *s;
    std::vector<Resource>::const_iterator it;

    for(it = surfaces.begin(); it != surfaces.end(); ++it) {
        if (it->filename == path)
            return it->surface;
    }

    //We need to load a new surface

    r.surface = IMG_Load( path.c_str() );

    if (!r.surface) {
        std::cout << "ERROR :: Loading surface '" << path << "'\n";
        return NULL;
    }

    r.filename = path;
    surfaces.push_back(r);

    return r.surface;
}

void
RSM::Init(void) {
}

void
RSM::CleanUp(void) {
    std::vector<Resource>::const_iterator it;

    for(it = surfaces.begin(); it != surfaces.end(); ++it)
        if (it->surface)
            SDL_FreeSurface(it->surface);
}
