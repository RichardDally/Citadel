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

    // Returns character picked to play
    Character HumanPlayer::PickCharacter(const std::unordered_set<Character>& remainingCards)
    {
        assert(remainingCards.size() > 0);
        return *std::begin(remainingCards);
    }

    // Returns action to be taken
    PlayerAction HumanPlayer::ChooseAction()
    {
        assert(!"Boom");
        return PlayerAction::UNITIALIZED;
    }

    // Returns district card id player wants to build
    std::vector<int> HumanPlayer::ChooseDistrictCardsToBuild(const size_t authorizedBuilds)
    {
        assert(!"Boom");
        return std::vector<int>();
    }

    // Returns character targeted by assassination or theft
    Character HumanPlayer::ChooseCharacterTarget()
    {
        assert(!"Boom");
        return Character::UNINITIALIZED;
    }

    // Returns opponent player id, current player wants to target
    int HumanPlayer::ChoosePlayerTarget(std::vector<const Player*> opponents)
    {
        assert(!"Boom");
        return -1;
    }

    // Returns a choice specific to Magician character
    MagicianChoice HumanPlayer::MagicianDecision()
    {
        assert(!"Boom");
        return MagicianChoice::DO_NOTHING;
    }
}