#pragma once

#include "../Tile/TileType.h"
#include "../Entity/EntityType.h"

#include "../Common/Vector2Int.h"

#include "ResourceManagers.h"
#include "Sprite2D.h"

namespace Agvt
{
    class SpriteLoader
    {
    public:
        SpriteLoader();
        SpriteLoader(std::shared_ptr<Camera> camera);

        std::unique_ptr<Sprite2D> LoadTileSprite(TileType tileType, uint8_t tileDetails, Vector2Int position = {}) const;
        std::unique_ptr<Sprite2D> LoadEntitySprite(EntityType entityType, Vector2Int position = {}) const;

    private:
        std::shared_ptr<Model> m_spriteModel;
        std::shared_ptr<Shader> m_spriteShader;
        std::shared_ptr<Shader> m_atlasSpriteShader;

        std::shared_ptr<Camera> m_camera;
    };
}
