#include "party.hpp"
#include "resource.hpp"

Party::Party(void) {
    loc_indicator = RSM::GetSurface("res/marker.png");

    characters[0] = new Character("Sir William", Species::HUMAN, CharacterClass::PALADIN);
    characters[1] = new Character("Grosk", Species::ORC, CharacterClass::FIGHTER);
    characters[2] = new Character("Marcus Lightburn", Species::HUMAN, CharacterClass::CLERIC);
    characters[3] = new Character("Leoric", Species::HALF_ELF, CharacterClass::ROGUE);
    characters[4] = new Character("Sothwyr", Species::ELF, CharacterClass::WIZARD);
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
