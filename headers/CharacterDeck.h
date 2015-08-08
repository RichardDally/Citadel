#pragma once

#include <vector>
#include "CharacterCard.h"

class CharacterDeck
{
public:
    CharacterDeck() = default;

    void Setup(std::initializer_list<Character>&& availableCharacters, const size_t numberOfPlayers)
    {
        availableCharacters_ = availableCharacters;
        numberOfPlayers_ = numberOfPlayers;
        visibleCards_.clear();
        availableCards_.clear();
    }

    void StartANewRound()
    {

    }

private:
    size_t numberOfPlayers_ = 0;
    std::vector<const Character> availableCharacters_;

    std::vector<const Character> visibleCards_;
    std::vector<const Character> availableCards_;
};
