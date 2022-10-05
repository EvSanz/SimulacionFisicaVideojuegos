#include "Plano.h"

Plano::Plano(Vector3 position, Vector4 color)
{
	pos = physx::PxTransform(position.x, position.y, position.z);
	renderItem = new RenderItem(CreateShape(physx::PxBoxGeometry(500, 0.1, 500)), &pos, color);
}

Plano::~Plano()
{
	renderItem = nullptr;
	DeregisterRenderItem(renderItem);
}
