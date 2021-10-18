#pragma once

#include <stdint.h>

namespace Agvt
{
    namespace GameWorldConfig
    {
        constexpr uint8_t c_mapTileWidth = 10;
        constexpr uint8_t c_mapTileHeight = 10;

        constexpr float c_mapWidth = 10.0f;
        constexpr float c_mapHeight = 10.0f;
        constexpr float c_tileSize = 1.0f;
        constexpr float c_tileAlign = c_tileSize * 0.5f;
    }
}
