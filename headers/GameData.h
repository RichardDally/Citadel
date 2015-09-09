#pragma once

namespace Citadel
{
#pragma region Edition
    enum class Edition
    {
        UNINITIALIZED,
        REGULAR,
        REGULAR_WITHOUT_PURPLE_DISTRICTS,
        DARK_CITY,
        MAX
    };
    const char* const GetEditionName(const Edition edition);
#pragma endregion
}