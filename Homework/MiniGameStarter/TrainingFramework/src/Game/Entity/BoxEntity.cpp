#include "BoxEntity.h"

namespace Agvt
{
	BoxEntity::BoxEntity(std::unique_ptr<Sprite2D> sprite) : Entity(EntityType::Box, std::move(sprite))
	{
	}
}
