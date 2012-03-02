#include "party.hpp"
#include "resource.hpp"

Party::Party(void) {
    loc_indicator = RSM::GetSurface("res/marker.png");
}

Party::~Party(void) {
    // Don't fere the loc_indicator now as this is handled by RSM::CleanUp()
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
    location->CentreCamera();

    //Update location indicator location
}
