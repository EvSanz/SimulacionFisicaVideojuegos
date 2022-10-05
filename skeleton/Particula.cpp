#include "Particula.h"

using namespace physx; 

Particula::Particula(Vector3 posicion, Vector3 velocidad, Vector3 aceleracion, float damping, float mass, int disparo, Vector4 color)
{ 
	vel = velocidad; 
	acc = aceleracion; 
	damp = damping; 
	masa = mass; 
	time = 2.0f; 

	pos = PxTransform(posicion.x, posicion.y, posicion.z); 

	switch (disparo)
	{
	case 1:
		renderItem = new RenderItem(CreateShape(PxSphereGeometry(2)), &pos, color);
		break;
	case 2:
		renderItem = new RenderItem(CreateShape(PxCapsuleGeometry(2, 5)), &pos, color);
		break;
	case 3:
		renderItem = new RenderItem(CreateShape(PxBoxGeometry(1, 1, 1)), &pos, color);
		break;
	default: 
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
	time -= t;

	vel = Vector3(vel.x + acc.x * t, vel.y + acc.y * t, vel.z + acc.z * t);
	vel *= pow(damp, t); 
	pos = PxTransform(pos.p.x + vel.x * t, pos.p.y + vel.y * t, pos.p.z + vel.z * t);

	if (pos.p.y < 0 || time < 0)
		killParticle(); 
}
