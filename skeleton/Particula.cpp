#include "Particula.h" 

Particula::Particula(ClasesParticulas p)
{ 
	pos = p.pose;
	vel = p.vel; 
	acc = p.acc; 
	size = p.size; 
	damp = p.damp;  
	time = p.time;
	force = p.force; 

	if (p.mass > 0.0f)
		inverse_mass = 1.0f / p.mass;
	else
		inverse_mass = 0.0f; 

	mass = p.mass;
	tipoClase = p; 

	if (p.disparo == 1)
		renderItem = new RenderItem(CreateShape(PxSphereGeometry(size.x)), &pos, p.color);
	else if (p.disparo == 2)
		renderItem = new RenderItem(CreateShape(PxBoxGeometry(size.x, size.y, size.z)), &pos, p.color);
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

	pos.p += vel * t; 

	auto totalacc = force * inverse_mass;
	vel = vel * pow(damp, t) + totalacc * t;

	time -= t;

	if (time < 0)
		killParticle();

	clearForce();
}

