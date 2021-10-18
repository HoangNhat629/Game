#include "RockEntity.h"

namespace Agvt
{
	RockEntity::RockEntity(std::unique_ptr<Sprite2D> sprite) : Entity(EntityType::Rock, std::move(sprite))
	{
	}
}
