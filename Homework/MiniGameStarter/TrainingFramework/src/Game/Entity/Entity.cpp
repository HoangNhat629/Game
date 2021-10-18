#include "Entity.h"

#include "../GameWorldConfig.h"

#include "ResourceManagers.h"

#include "Sprite2D.h"
#include "AtlasSprite2D.h"

namespace Agvt
{
    using namespace GameWorldConfig;

    void Entity::SetPosition(Vector2Int gridPosition)
    {
        m_gridPosition = gridPosition;
        m_sprite->Set2DPosition(Vector2(gridPosition.x + c_tileAlign, gridPosition.y + c_tileAlign));
    }

    void Entity::SetRotation(Vector3 rotation)
    {
        m_sprite->SetRotation(rotation);
        m_sprite->CalculateWorldMatrix();
    }

    void Entity::SetCamera(std::shared_ptr<Camera> camera)
    {
        m_sprite->SetCamera(camera);
    }
}
