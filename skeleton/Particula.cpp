#include "Particula.h"

using namespace physx; 

Particula::Particula(Vector3 posicion, Vector3 velocidad, Vector3 aceleracion, float damping, int disparo)
{ 
	vel = velocidad; 
	acc = aceleracion; 
	damp = damping; 

	pos = PxTransform(posicion.x, posicion.y, posicion.z); 

	switch (disparo)
	{
	case 1:
		renderItem = new RenderItem(CreateShape(PxSphereGeometry(5)), &pos, { 1, 1, 0, 1 });
		break;
	case 2:
		renderItem = new RenderItem(CreateShape(PxCapsuleGeometry(2, 5)), &pos, { 1, 1, 0, 1 });
		break;
	default:
		//renderItem = new RenderItem(CreateShape(PxBoxGeometry(5, 5)), &pos, { 0, 1, 0, 1 });
		break;
	}
}

Particula::~Particula()
{
	renderItem = nullptr; 
	DeregisterRenderItem(renderItem); 
}

void Particula::integrate(double t)
{
	pos = PxTransform(pos.p.x + vel.x * t, pos.p.y + vel.y * t, pos.p.z + vel.z * t); 
	vel += acc * t; 
	vel *= pow(damp, t); 
}
