#include <random>
#include "Randomness.h"

namespace
{
    std::random_device rd;
    std::mt19937 mt(rd()); // seed the random number generator once
}

namespace Citadel
{
    namespace Dice
    {
        size_t GetRandomNumber(const size_t lowBound, const size_t highBound)
        {
            std::uniform_int_distribution<size_t> dist(lowBound, highBound);
            return dist(mt);
        }
    }
}