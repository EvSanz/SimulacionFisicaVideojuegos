#include <map>
#include <iostream>

#include "ForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(Vector3& gravedad)
{
	gravity = gravedad; 
}

void GravityForceGenerator::updateForce(Particula* p, double t)
{
	if (fabs(p->getInverseMass()) < 1e-10)
		return; 

	p->addForce(gravity * p->getMass()); 
}

///////////////////////////////////////////////////////////////////

WindForceGenerator::WindForceGenerator(const Vector3& vel, const Vector3& pos, double r) 
{
	velViento = vel; 
	posViento = pos; 
	rViento = r; 
}

void WindForceGenerator::updateForce(Particula* p, double t)
{
	if ((fabs(p->getInverseMass()) < 1e-10) || (p->getPos() - posViento).magnitudeSquared() > pow(rViento, 2))
		return;

	p->addForce(0.003 * velViento + 0.002 * velViento.magnitude() * velViento);
}

///////////////////////////////////////////////////////////////////

TornadeForceGenerator::TornadeForceGenerator(const Vector3& pos, double r, double aux)
{
	rafaga = new WindForceGenerator({0, 5, 0}, pos, r);
	k = aux; 
}

Vector3 TornadeForceGenerator::getVel(Vector3 p)
{
	return k * Vector3(-(p.z - rafaga->getPos().z), 50 - (p.y - rafaga->getPos().y), p.x - rafaga->getPos().x);
}

///////////////////////////////////////////////////////////////////

ExplosionForceGenerator::ExplosionForceGenerator(const Vector3& pos, double r, double duracion)
{
	expPos = pos; 
	rExplosion = r; 
	tiempo = duracion; 
}

void ExplosionForceGenerator::updateForce(Particula* p, double t)
{
	if (t >= tiempo || (fabs(p->getInverseMass()) < 1e-10) 
		            || (p->getPos() - expPos).magnitudeSquared() > pow(rExplosion, 2))
		return;

	tiempo += t; 

	p->addForce((8000 / pow(rExplosion, 2)) * (p->getPos() - expPos) * pow(2.71828, -((t / 1000.0))));
}

///////////////////////////////////////////////////////////////////


void ParticleDragGenerator::updateForce(Particula* p, double t)
{
	if (fabs(p->getInverseMass()) < 1e-10)
		return; 

	Vector3 v = p->getVel(), dragF; 

	float drag_coef = v.normalize(); 
	drag_coef = _k1 * drag_coef + _k2 * drag_coef * drag_coef; 
	dragF = -v * drag_coef; 

	std::cout << dragF.x << "\t" << dragF.y << "\t" << dragF.z << "\n"; 
	p->addForce(dragF); 
}