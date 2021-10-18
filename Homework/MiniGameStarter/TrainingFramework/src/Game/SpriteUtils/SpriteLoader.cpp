#include "SpriteLoader.h"

#include "../GameWorldConfig.h"
#include "SpriteResourceConfig.h"

#include "SpriteTraits.h"

#include "Sprite2D.h"
#include "AtlasSprite2D.h"

#include <assert.h>

namespace Agvt
{
    using namespace GameWorldConfig;

    SpriteLoader::SpriteLoader() : m_camera{}
    {
        auto resourceManager = ResourceManagers::GetInstance();

        m_spriteModel = resourceManager->GetModel("Sprite2D-cartesian.nfg");
        m_spriteShader = resourceManager->GetShader("TextureShader");
        m_atlasSpriteShader = resourceManager->GetShader("AtlasTextureShader");
    }

    SpriteLoader::SpriteLoader(std::shared_ptr<Camera> camera) : SpriteLoader()
    {
        m_camera = std::move(camera);
    }

    std::unique_ptr<Sprite2D> SpriteLoader::LoadTileSprite(TileType tileType, uint8_t tileDetails, Vector2Int position) const
    {
        auto resourceManager = ResourceManagers::GetInstance();

        std::shared_ptr<Texture> tileTexture = ([&resourceManager, tileType, tileDetails]()
        {
            switch (tileType)
            {
            case TileType::Wall:
                if (tileDetails == 1)
                {
                    return resourceManager->GetTexture(SpriteResourceConfig::TileWall1, true);
                }
                else
                {
                    return resourceManager->GetTexture(SpriteResourceConfig::TileWall0, true);
                }
                break;
            case TileType::Exit:
                return resourceManager->GetTexture(SpriteResourceConfig::TileDoor, true);
            default:
                assert(false && "Unknown tile type");
                return std::shared_ptr<Texture>{};
            }
        })();


        std::unique_ptr<Sprite2D> tileSprite;
        auto atlasSize = SpriteTraits::GetAtlasSpriteSize(tileType, tileDetails);
        if (atlasSize)
        {
            tileSprite = std::make_unique<AtlasSprite2D>(m_spriteModel, m_atlasSpriteShader, tileTexture, *atlasSize);
        }
        else
        {
            tileSprite = std::make_unique<Sprite2D>(m_spriteModel, m_spriteShader, tileTexture);
        }

        if (m_camera != nullptr)
        {
            tileSprite->SetCamera(m_camera);
        }
        tileSprite->SetSize(c_tileSize, c_tileSize);
        tileSprite->Set2DPosition(Vector2(position.x + c_tileAlign, position.y + c_tileAlign));

        return tileSprite;
    }

    std::unique_ptr<Sprite2D> SpriteLoader::LoadEntitySprite(EntityType entityType, Vector2Int position) const
    {
        auto resourceManager = ResourceManagers::GetInstance();

        std::shared_ptr<Texture> entityTexture = ([&resourceManager, entityType]()
        {
            switch (entityType)
            {
            case EntityType::Player:
                return resourceManager->GetTexture(SpriteResourceConfig::PlayerEntity, true);
            case EntityType::Rock:
                return resourceManager->GetTexture(SpriteResourceConfig::RockEntity, true);
            case EntityType::Box:
                return resourceManager->GetTexture(SpriteResourceConfig::BoxEntity, true);
            case EntityType::Key:
                return resourceManager->GetTexture(SpriteResourceConfig::KeyEntity, true);
            default:
                assert(false && "Unknown entity type");
                return std::shared_ptr<Texture>{};
            }
        })();

        std::unique_ptr<Sprite2D> entitySprite;
        auto atlasSize = SpriteTraits::GetAtlasSpriteSize(entityType);
        if (atlasSize)
        {
            entitySprite = std::make_unique<AtlasSprite2D>(m_spriteModel, m_atlasSpriteShader, entityTexture, *atlasSize);
        }
        else
        {
            entitySprite = std::make_unique<Sprite2D>(m_spriteModel, m_spriteShader, entityTexture);
        }

        if (m_camera != nullptr)
        {
            entitySprite->SetCamera(m_camera);
        }
        entitySprite->SetSize(c_tileSize, c_tileSize);
        entitySprite->Set2DPosition(Vector2(position.x + c_tileAlign, position.y + c_tileAlign));

        return entitySprite;
    }

}