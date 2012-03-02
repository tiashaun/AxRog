#include "tile.hpp"
#include "resource.hpp"

Tile::Tile(void) {
    visible = false;
}

Tile::~Tile(void) {
    //Don't free tile.surf here as it's handled by RSM::CleanUp();
}

void
Tile::MakeWall(void) {
    surf = RSM::GetSurface("res/tiles/wall.png");
    isUnusedSpace = true;
}

void
Tile::MakeFloor(void) {
    surf = RSM::GetSurface("res/tiles/floor.png");
    isUnusedSpace = false;
}
