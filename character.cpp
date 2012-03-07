#include "character.hpp"
#include "resource.hpp"
#include "fonthandler.hpp"

#define PARTY_SCREEN_COLUMN_WIDTH   24
#define PARTY_SCREEN_COLUMN_OFFSET  6

#define LENGTH(x) (sizeof (x) / sizeof(x[0]))

#include <sstream>

Character::Character(std::string name, Species::Type inSpecies,
                                            CharacterClass::Type inClass) {
    this->species = inSpecies;
    this->cclass = inClass;
    this->name = name;
    this->level = 1;
    this->XP = 0;

    this->headgear = NULL;
    this->armour = NULL;
    this->shield = NULL;
    this->weapon = NULL;
    this->boots = NULL;
    this->misc = NULL;

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
            // this->spd += 0;
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
            // this->spd += 0;
            //TODO: Add basic spells
            break;
        case CharacterClass::PALADIN :
            this->max_hp += 7;
            this->max_mp = 5;
            this->att += 4;
            this->def += 2;
            this->str += 4;
            this->tou += 4;
            this->wil += 4;
            // this->spd += 0;
        default:
            break;
    }

    //Startign equipment
    switch(this->cclass) {
        case CharacterClass::PALADIN :
        case CharacterClass::FIGHTER :
            this->weapon = Equippable::GetItem("Longsword");
            this->armour = Equippable::GetItem("Chainmail");
            break;
        case CharacterClass::ROGUE :
            this->weapon = Equippable::GetItem("Shortsword");
            this->armour = Equippable::GetItem("Leather Armour");
            break;
        case CharacterClass::CLERIC :
            this->weapon = Equippable::GetItem("Mace");
            this->armour = Equippable::GetItem("Chainmail");
            break;
        case CharacterClass::WIZARD :
            this->armour = Equippable::GetItem("Robes");
            break;
        default:
            break;
    }


    // Attribute modifiers come from species
    switch (this->species) {
        case Species::HUMAN :
            break;
        case Species::ELF :
            this->max_hp -= 5;
            this->att += 1;
            this->def += 1;
            this->tou -= 2;
            this->spd += 2;
            if (max_mp)
                max_mp += 5;
            break;
        case Species::HALF_ELF :
            this->max_hp -= 2;
            this->tou -= 1;
            this->spd += 1;
            if (max_mp)
                max_mp += 2;
            break;
        case Species::ORC :
            this->max_hp += 2;
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
        case CharacterClass::PALADIN :
            this->portrait = RSM::GetSurface("res/portraits/bas_paladin.png");
            break;
        default:
            this->portrait = RSM::GetSurface("res/portraits/other.png");
            break;
    }

    //Start us off healthy
    this->curr_hp = this->max_hp;
    this->curr_mp = this->max_mp;

    TotalGearModifiers();
}

void
Character::TotalGearModifiers(void) {
    int i;
    Equippable::EItem *gear[] = {
        this->headgear, 
        this->armour, 
        this->shield,
        this->weapon,
        this->boots,
        this->misc
    };

    this->tot_att = this->att;
    this->tot_def = this->def;
    this->tot_str = this->str;
    this->tot_tou = this->tou;
    this->tot_wil = this->wil;
    this->tot_spd = this->spd;

    for (i = 0; i < LENGTH(gear); ++i) {
        if (gear[i]) {
            tot_att += gear[i]->att_mod;
            tot_def += gear[i]->def_mod;
            tot_str += gear[i]->str_mod;
            tot_tou += gear[i]->tou_mod;
            tot_wil += gear[i]->wil_mod;
            tot_spd += gear[i]->spd_mod;
        }
    }
}

void
Character::DrawPartyScreenLine(SDL_Surface *surf, SDL_Rect dest) {
    std::stringstream ss;
    std::string species_str;
    std::string class_str;
    SDL_Rect relative;
    SDL_BlitSurface(this->portrait, NULL, surf, &dest);

    //Race string
    if (this->species == Species::HUMAN)
        species_str = "Human";
    else if (this->species == Species::ELF)
        species_str = "Elven";
    else if (this->species == Species::HALF_ELF)
        species_str = "Helf-Elven";
    else if (this->species == Species::ORC)
        species_str = "Orcish";
    else
        species_str = "";

    //Class string
    if (this->cclass == CharacterClass::FIGHTER)
        class_str = "Fighter";
    else if (this->cclass == CharacterClass::ROGUE)
        class_str = "Rogue";
    else if (this->cclass == CharacterClass::CLERIC)
        class_str = "Cleric";
    else if (this->cclass == CharacterClass::WIZARD)
        class_str = "Wizard";
    else if (this->cclass == CharacterClass::PALADIN)
        class_str = "Paladin";
    else
        class_str = "";

    //First block of text is personal info
    relative = dest;
    relative.x += 6 * BLOCK_SIZE;
    FontHandler::WriteText(surf, relative, this->name);
    relative.y += BLOCK_SIZE;
    FontHandler::WriteText(surf, relative, species_str + " " + class_str);
    relative.y += BLOCK_SIZE;
    ss.str("");
    ss << "Level: " << this->level;
    FontHandler::WriteText(surf, relative, ss.str());
    relative.y += BLOCK_SIZE;
    ss.str("");
    ss << "XP: " << this->XP;
    FontHandler::WriteText(surf, relative, ss.str());
    relative.y += BLOCK_SIZE;
    relative.y += BLOCK_SIZE;
    ss.str("");
    ss << "HP: " << this->curr_hp << "/" << this->max_hp;
    FontHandler::WriteText(surf, relative, ss.str());
    relative.y += BLOCK_SIZE;
    ss.str("");
    ss << "MP: " << this->curr_mp << "/" << this->max_mp;
    FontHandler::WriteText(surf, relative, ss.str());

    //Second block of text is stats
    relative = dest;
    relative.x += 20 * BLOCK_SIZE;
    relative.y += BLOCK_SIZE;
    ss.str("");
    ss << "Att: " << this->tot_att;
    FontHandler::WriteText(surf, relative, ss.str());
    relative.y += BLOCK_SIZE;
    ss.str("");
    ss << "Def: " << this->tot_def;
    FontHandler::WriteText(surf, relative, ss.str());
    relative.y += BLOCK_SIZE;
    ss.str("");
    ss << "Str: " << this->tot_str;
    FontHandler::WriteText(surf, relative, ss.str());
    relative.y += BLOCK_SIZE;
    ss.str("");
    ss << "Tou: " << this->tot_tou;
    FontHandler::WriteText(surf, relative, ss.str());
    relative.y += BLOCK_SIZE;
    ss.str("");
    ss << "Wil: " << this->tot_wil;
    FontHandler::WriteText(surf, relative, ss.str());
    relative.y += BLOCK_SIZE;
    ss.str("");
    ss << "Spd: " << this->tot_spd;
    FontHandler::WriteText(surf, relative, ss.str());


    //Third block of text is equipment
    relative = dest;
    relative.x += 30 * BLOCK_SIZE;
    relative.y += BLOCK_SIZE;
    ss.str("");
    ss << "Headgear: ";
    if (this->headgear)
        ss << this->headgear->name;
    else
        ss << "None";
    FontHandler::WriteText(surf, relative, ss.str());
    relative.y += BLOCK_SIZE;
    ss.str("");
    ss << "Armour:   ";
    if (this->armour)
        ss << this->armour->name;
    else
        ss << "None";
    FontHandler::WriteText(surf, relative, ss.str());
    relative.y += BLOCK_SIZE;
    ss.str("");
    ss << "Shield:   ";
    if (this->shield)
        ss << this->shield->name;
    else
        ss << "None";
    FontHandler::WriteText(surf, relative, ss.str());
    relative.y += BLOCK_SIZE;
    ss.str("");
    ss << "Weapon:   ";
    if (this->weapon)
        ss << this->weapon->name;
    else
        ss << "None";
    FontHandler::WriteText(surf, relative, ss.str());
    relative.y += BLOCK_SIZE;
    ss.str("");
    ss << "Boots:    ";
    if (this->boots)
        ss << this->boots->name;
    else
        ss << "None";
    FontHandler::WriteText(surf, relative, ss.str());
    relative.y += BLOCK_SIZE;
    ss.str("");
    ss << "Misc:     ";
    if (this->misc)
        ss << this->misc->name;
    else
        ss << "None";
    FontHandler::WriteText(surf, relative, ss.str());
}

Character::~Character(void) {
    // The portrait is freed by the resource manager

    // Not sure how items are going to be handled so this is tentatice

    // if (this->helm)
    //     delete helm;
    // if (this->weapon)
    //     delete helm;
    // if (this->armour)
    //     delete helm;
    // if (this->misc1)
    //     delete helm;
    // if (this->misc2)
    //     delete helm;
}
