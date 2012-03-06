#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include <vector>
#include <SDL/SDL.h>

struct Resource {
    std::string filename;
    SDL_Surface *surface;
};

namespace RSM {
    SDL_Surface* GetSurface(std::string path);
    void Init(void);
    void CleanUp(void);

    static std::vector<Resource> surfaces;
};

#endif
