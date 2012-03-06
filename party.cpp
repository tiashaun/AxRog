#include "party.hpp"
#include "resource.hpp"

Party::Party(void) {
    loc_indicator = RSM::GetSurface("res/marker.png");

    characters[0] = new Character(Species::ORC, CharacterClass::FIGHTER);
    characters[1] = new Character(Species::HUMAN, CharacterClass::CLERIC);
    characters[2] = new Character(Species::HALF_ELF, CharacterClass::ROGUE);
    characters[3] = new Character(Species::ELF, CharacterClass::WIZARD);
    characters[4] = NULL;
    characters[5] = NULL;
}

Party::~Party(void) {
    // Don't free the loc_indicator now as this is handled by RSM::CleanUp()
    for (int i = 0; i < MAX_PARTY_SIZE; ++i) {
        if (characters[i])
            delete characters[i];
    }
}

void
Party::SetLocation(Room *r) {
    location = r;
    r->Reveal();
}

void
Party::Travel(Direction d) {
    if ( !location->hasPathInDirection(d) ) {
        location->CentreCamera();
        return;
    }
    location->RevealPath(d);
    location->RevealRoomInDirection(d);
    location = location->GetRoomInDirection(d);
    location->CentreCamera();

    //Update location indicator location
}
