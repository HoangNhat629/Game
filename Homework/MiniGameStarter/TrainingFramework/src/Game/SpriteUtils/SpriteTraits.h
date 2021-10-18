#pragma once

#include "../Tile/TileType.h"
#include "../Entity/EntityType.h"

#include "Common/Vector2Int.h"

#include <optional>

namespace Agvt
{
    struct SpriteTraits
    {
        static constexpr std::optional<Vector2Int> GetAtlasSpriteSize(TileType tileType, uint8_t tileDetails)
        {
            switch (tileType)
            {
            case TileType::Exit:
                return { Vector2Int(2, 1) };

            case TileType::Empty:
            case TileType::Wall:
            default:
                return std::nullopt;
            }
        }

        static constexpr std::optional<Vector2Int> GetAtlasSpriteSize(EntityType entityType)
        {
            switch (entityType)
            {
            case EntityType::Player:
                return { Vector2Int(6, 2) };

            case EntityType::Rock:
            case EntityType::Box:
            case EntityType::Key:
            default:
                return std::nullopt;
            }
        }
    };

}