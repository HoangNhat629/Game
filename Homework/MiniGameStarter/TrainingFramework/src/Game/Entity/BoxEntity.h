#pragma once

#include "Entity.h"

namespace Agvt
{
	class BoxEntity : public Entity
	{
	public:
		BoxEntity(std::unique_ptr<Sprite2D> sprite);
	};
}
