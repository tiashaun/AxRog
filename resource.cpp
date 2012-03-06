#include "resource.hpp"
#include <SDL/SDL_image.h>

namespace RSM {
    SDL_Surface*
    GetSurface(std::string path) {
        Resource r;
        SDL_Surface *s;
        std::vector<Resource>::const_iterator it;

        for(it = surfaces.begin(); it != surfaces.end(); ++it) {
            if (it->filename == path)
                return it->surface;
        }

        //We need to load a new surface

        r.filename = path;
        r.surface = IMG_Load( path.c_str() );
        surfaces.push_back(r);

        return r.surface;
    }

    void
    Init(void) {
    }

    void
    CleanUp(void) {
        std::vector<Resource>::const_iterator it;

        for(it = surfaces.begin(); it != surfaces.end(); ++it)
            SDL_FreeSurface(it->surface);
    }
}
