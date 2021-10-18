#include "KeyEntity.h"

namespace Agvt
{
	KeyEntity::KeyEntity(std::unique_ptr<Sprite2D> sprite) : Entity(EntityType::Key, std::move(sprite))
	{
	}
}
