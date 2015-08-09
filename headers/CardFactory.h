#pragma once

#include <map>
#include <memory>

#include "CharacterCard.h"

const CharacterCard& GetCharacterCard(const Character character)
{
    using Key = Character;
    using Value = CharacterCard;
    static std::map<Key, Value> cache;

    auto it = cache.find(character);
    if (it != cache.end())
    {
        return it->second;
    }
    else
    {
        cache.insert(std::make_pair(character, CharacterCard(character)));
    }
}