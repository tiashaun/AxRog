#include "tile.hpp"
#include "resource.hpp"

Tile::Tile(void) {
    visible = true;
}

void
Tile::load(std::string str) {
    surf = RSM::GetSurface(str);
}
