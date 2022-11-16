#include "Particula.h" 

Particula::Particula(ClasesParticulas p)
{ 
	pos = p.pose;
	vel = p.vel; 
	acc = p.acc; 
	size = p.size; 
	damp = p.damp;  
	time = p.time;

	if (p.mass > 0.0f)
		inverse_mass = 1.0f / p.mass;
	else
		inverse_mass = 0.0f; 

	mass = p.mass;
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
	if (inverse_mass <= 0.0f)
		return; 

	auto totalacc = acc + force * inverse_mass;
	
	vel += totalacc * t;
	vel *= pow(damp, t);

	pos = PxTransform(pos.p.x + vel.x * t, pos.p.y + vel.y * t, pos.p.z + vel.z * t);


	time -= t;

	if (time < 0)
		killParticle();

	clearForce();
}

