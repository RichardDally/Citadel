#pragma once

#include "Card.h"
#include "GameData.h"

class CharacterCard : public Card
{
public:
    CharacterCard(const Character character)
        : Card(GetCharacterName(character), GetCharacterDescription(character)),
        character_(character)
    {
    }

    const Character GetCharacter() const
    {
        return character_;
    }

private:
    const Character character_;
};