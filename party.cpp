#include "party.hpp"

Party::Party(void) {
}

Party::~Party(void) {
}

void
Party::SetLocation(Room *r) {
    location = r;
    r->Reveal();
}

void
Party::Travel(Direction d) {
    if ( !location->hasPathInDirection(d) )
        return;
    location->RevealPath(d);
    location->RevealRoomInDirection(d);
    location = location->GetRoomInDirection(d);
}
