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
	if (inverse_mass <= 0.0)
		return; 

	time -= t;

	pos = PxTransform(pos.p.x + vel.x * t, pos.p.y + vel.y * t, pos.p.z + vel.z * t);

	Vector3 totalacc = Vector3(acc.x + force.x * inverse_mass, acc.y + force.y * inverse_mass, acc.z + force.z * inverse_mass);

	vel = Vector3(vel.x + totalacc.x * t, vel.y + totalacc.y * t, vel.z + totalacc.z * t); 
	vel *= powf(damp, t);

	clearForce(); 

	if (time < 0)
		killParticle(); 
}

