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
    static std::vector<Resource> surfaces;
    SDL_Surface* GetSurface(std::string path);
    void Init(void);
    void CleanUp(void);
};

#endif
