#include "HumanPlayer.h"

namespace Citadel
{
    HumanPlayer::HumanPlayer()
    {
    }

    HumanPlayer::HumanPlayer(const std::string& name)
        : Player(name)
    {
    }

    Character HumanPlayer::PickRoleDecision(const std::unordered_set<Character>& remainingCards)
    {
        assert(remainingCards.size() > 0);
        return *std::begin(remainingCards);
    }

    void HumanPlayer::PlayRoleStep()
    {
    }
}