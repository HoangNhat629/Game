#pragma once

#include "../Entity/EntityType.h"
#include "../Tile/TileType.h"
#include "../Map/GameMap.h"

#include "Sprite2D.h"

#include <memory>

namespace Agvt
{
    class EditorGridSlot
    {
    public:
        enum class ContentType : uint8_t
        {
            Empty,
            Tile,
            Entity
        };

        union Container
        {
            uint8_t tileData;
            EntityType entityType;
            uint8_t bits;
        };

    public:
        EditorGridSlot() : m_contentType(ContentType::Empty), m_container{}
        {
            static_assert(sizeof(Container) == 1);
        }

        EditorGridSlot(TileType tileType, uint8_t tileDetails, std::unique_ptr<Sprite2D> sprite) : m_contentType(ContentType::Tile), m_sprite(std::move(sprite))
        {
            m_container.tileData = GameMap::PackTileInfo((uint8_t)tileType, tileDetails);
        }

        EditorGridSlot(EntityType entityType, std::unique_ptr<Sprite2D> sprite) : m_contentType(ContentType::Entity), m_sprite(std::move(sprite))
        {
            m_container.entityType = entityType;
        }

    private:
        EditorGridSlot(const EditorGridSlot& original, std::unique_ptr<Sprite2D> newSprite) :
            m_contentType(original.m_contentType), m_container(original.m_container), m_sprite(std::move(newSprite))
        {
        }

    public:
        ContentType GetContentType() const
        {
            return m_contentType;
        }

        EntityType GetEntityType() const
        {
            return m_container.entityType;
        }

        std::pair<TileType, uint8_t> GetTileDataUnpacked() const
        {
            auto [tileType, tileDetails] = GameMap::UnpackTileData(m_container.tileData);
            return std::make_pair((TileType)tileType, tileDetails);
        }
        uint8_t GetTileDataPacked() const
        {
            return m_container.tileData;
        }

        Sprite2D* GetSprite() const
        {
            return m_sprite.get();
        }

        EditorGridSlot Clone() const
        {
            return EditorGridSlot(*this, m_sprite->Clone());
        }

        friend bool operator==(const EditorGridSlot& left, const EditorGridSlot& right)
        {
            return left.m_contentType == right.m_contentType && left.m_container.bits == right.m_container.bits;
        }

    private:
        ContentType m_contentType;
        Container m_container;

        std::unique_ptr<Sprite2D> m_sprite;
    };

}
