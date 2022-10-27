#include "Particula.h" 

Particula::Particula(ClasesParticulas p)
{ 
	pos = p.pose;
	vel = p.vel; 
	acc = p.acc; 
	size = p.size; 
	damp = p.damp; 
	mass = p.mass; 
	time = p.time;

	tipoClase = p; 

	switch (p.disparo)
	{
	case 1:
		renderItem = new RenderItem(CreateShape(PxSphereGeometry(size.x)), &pos, p.color);
		break;
	case 2:
		renderItem = new RenderItem(CreateShape(PxCapsuleGeometry(size.x, size.y)), &pos, p.color);
		break;
	case 3:
		renderItem = new RenderItem(CreateShape(PxBoxGeometry(size.x, size.y, size.z)), &pos, p.color);
		break;
	default: 
		break; 
	}
}

Particula::~Particula()
{
	DeregisterRenderItem(renderItem); 
	renderItem = nullptr;
}

void Particula::integrate(double t)
{
	time -= t;

	
	pos = PxTransform(pos.p.x + vel.x * t, pos.p.y + vel.y * t, pos.p.z + vel.z * t);
	vel = Vector3(vel.x + acc.x * t, vel.y + acc.y * t, vel.z + acc.z * t);
	vel *= pow(damp, t);

	if (/*pos.p.y < 0 ||*/ time < 0)
		killParticle(); 
}

