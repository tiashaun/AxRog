#include "character.hpp"
#include "resource.hpp"

Character::Character(Species::Type inSpecies, CharacterClass::Type inClass) {
    this->species = inSpecies;
    this->cclass = inClass;

    // Basic attributes
    this->max_health = 30;
    this->att = 10;
    this->def = 10;
    this->str = 10;
    this->tou = 10;
    this->wil = 10;
    this->spd = 10;

    // Attribute modifiers come from species
    switch (this->species) {
        case Species::HUMAN :
            break;
        case Species::ELF :
            this->max_health -= 5;
            this->att += 1;
            this->def += 1;
            this->tou -= 2;
            this->spd += 2;
            break;
        case Species::HALF_ELF :
            this->max_health -= 2;
            this->tou -= 1;
            this->spd += 1;
            break;
        case Species::ORC :
            this->att += 1;
            this->str += 2;
            this->tou += 1;
            this->wil -= 5;
            break;
        default:
            break;
    }

    // Attribute modifiers from class
    switch (this->cclass) {
        case CharacterClass::FIGHTER :
            this->max_health += 10;
            this->att += 5;
            this->def += 2;
            this->str += 5;
            this->tou += 5;
            // this->wil += 0;
            this->spd += 0;
            break;
        case CharacterClass::ROGUE :
            this->max_health += 5;
            this->att += 3;
            this->def += 5;
            this->str += 2;
            this->tou += 2;
            // this->wil += 0;
            this->spd += 2;
            break;
        case CharacterClass::CLERIC :
            this->max_health += 5;
            this->att += 2;
            this->def += 2;
            this->str += 2;
            this->tou += 2;
            this->wil += 5;
            this->spd += 2;
            //TODO: Add basic spells
            break;
        case CharacterClass::WIZARD :
            this->max_health += 0;
            this->att += 0;
            this->def += 0;
            this->str += 0;
            this->tou += 0;
            this->wil += 10;
            this->spd += 0;
            //TODO: Add basic spells
            break;
        default:
            break;
    }

    //Start us off healthy
    this->curr_health = this->max_health;

    //Load portraits
    this->portrait = RSM::GetSurface("res/portraits/other.png");
    this->combat_back = RSM::GetSurface("res/combat_back/other.png");
}
