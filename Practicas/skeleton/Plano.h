#pragma once

#include "RenderUtils.hpp"

class Plano
{
	public:
		Plano(Vector3 position, Vector4 color);
		~Plano();

	protected:
		physx::PxTransform pos;
		RenderItem* renderItem;
};

