#include "tile.hpp"
#include "resource.hpp"

void
Tile::load(std::string str) {
    surf = RSM::GetSurface(str);
}
