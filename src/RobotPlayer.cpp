#include "Randomness.h"
#include "RobotPlayer.h"

namespace
{
    const char* const names[] =
    {
        "Hal",
        "Synapse",
        "DevNull",
        "Alpha",
        "TheOracle"
    };
    const size_t namesSize = sizeof(names) / sizeof(names[0]);
}

namespace Citadel
{
    RobotPlayer::RobotPlayer()
        : Player(names[Dice::GetRandomNumber(0, namesSize - 1)])
    {
    }

    RobotPlayer::RobotPlayer(const std::string& name)
        : Player(name)
    {
    }

    // Returns character picked to play
    Character RobotPlayer::PickCharacter(const std::unordered_set<Character>& remainingCards)
    {
        assert(remainingCards.size() > 0);
        return *std::begin(remainingCards);
    }

    // Returns action to be taken
    PlayerAction RobotPlayer::ChooseAction()
    {
        assert(!"Boom");
        return PlayerAction::UNITIALIZED;
    }

    // Returns district card id player wants to build
    std::vector<int> RobotPlayer::ChooseDistrictCardsToBuild(const size_t authorizedBuilds)
    {
        assert(!"Boom");
        return std::vector<int>();
    }

    // Returns character targeted by assassination or theft
    Character RobotPlayer::ChooseCharacterTarget()
    {
        assert(!"Boom");
        return Character::UNINITIALIZED;
    }

    // Returns opponent player id, current player wants to target
    int RobotPlayer::ChoosePlayerTarget(std::vector<const Player*> opponents)
    {
        assert(!"Boom");
        return -1;
    }

    // Returns a choice specific to Magician character
    MagicianChoice RobotPlayer::MagicianDecision()
    {
        assert(!"Boom");
        return MagicianChoice::DO_NOTHING;
    }
}