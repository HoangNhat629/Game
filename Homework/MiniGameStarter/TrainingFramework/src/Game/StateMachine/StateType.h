#pragma once

#include <stdint.h>

namespace Agvt
{
    enum class StateType
    {
        Invalid,
        Playing,
        GravitySelecting,
        Updating,
        Ended
    };

    // Remember to update this when modifying StateType
    constexpr size_t c_stateTypeElementCount = 5;
}
