#pragma once

#include "Entity.h"

namespace Agvt
{
	class KeyEntity : public Entity
	{
	public:
		KeyEntity(std::unique_ptr<Sprite2D> sprite);
	};
}
