#include "character.hpp"
#include "resource.hpp"

Character::Character(Species::Type inSpecies, CharacterClass::Type inClass) {
    this->species = inSpecies;
    this->cclass = inClass;
    this->level = 1;
    this->XP = 0;

    // Basic attributes
    this->max_hp = 30;
    this->max_mp = 0;
    this->att = 10;
    this->def = 10;
    this->str = 10;
    this->tou = 10;
    this->wil = 10;
    this->spd = 10;

    // Attribute modifiers from class
    switch (this->cclass) {
        case CharacterClass::FIGHTER :
            this->max_hp += 10;
            this->att += 5;
            this->def += 2;
            this->str += 5;
            this->tou += 5;
            // this->wil += 0;
            this->spd += 0;
            break;
        case CharacterClass::ROGUE :
            this->max_hp += 5;
            this->att += 3;
            this->def += 5;
            this->str += 2;
            this->tou += 2;
            // this->wil += 0;
            this->spd += 2;
            break;
        case CharacterClass::CLERIC :
            this->max_hp += 5;
            this->max_mp = 20;
            this->att += 2;
            this->def += 2;
            this->str += 2;
            this->tou += 2;
            this->wil += 5;
            this->spd += 2;
            //TODO: Add basic spells
            break;
        case CharacterClass::WIZARD :
            this->max_hp += 0;
            this->max_mp = 30;
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

    // Attribute modifiers come from species
    switch (this->species) {
        case Species::HUMAN :
            this->name = "John Doe";
            break;
        case Species::ELF :
            this->name = "Leoric";
            this->max_hp -= 5;
            this->att += 1;
            this->def += 1;
            this->tou -= 2;
            this->spd += 2;
            if (max_mp)
                max_mp += 5;
            break;
        case Species::HALF_ELF :
            this->name = "Liamund";
            this->max_hp -= 2;
            this->tou -= 1;
            this->spd += 1;
            if (max_mp)
                max_mp += 2;
            break;
        case Species::ORC :
            this->name = "Grosk";
            this->att += 1;
            this->str += 2;
            this->tou += 1;
            this->wil -= 5;
            if (max_mp)
                max_mp -= 5;
            break;
        default:
            break;
    }

    //Start us off healthy
    this->curr_hp = this->max_hp;
    this->curr_mp = this->max_mp;

    //Load portraits
    switch (this->cclass) {
        case CharacterClass::FIGHTER :
            this->portrait = RSM::GetSurface("res/portraits/bas_fighter.png");
            break;
        case CharacterClass::ROGUE :
            this->portrait = RSM::GetSurface("res/portraits/bas_rogue.png");
            break;
        case CharacterClass::CLERIC :
            this->portrait = RSM::GetSurface("res/portraits/bas_cleric.png");
            break;
        case CharacterClass::WIZARD :
            this->portrait = RSM::GetSurface("res/portraits/bas_wizard.png");
            break;
        default:
            this->portrait = RSM::GetSurface("res/portraits/other.png");
            break;
    }
}

void
Character::DrawPartyScreenLine(SDL_Surface *surf, SDL_Rect *dest) {
    SDL_BlitSurface(this->portrait, NULL, surf, dest);
}
