#include <map>

#include "ForceGenerator.h"

void GravityForceGenerator::updateForce(Particula* p, double t)
{
	if (fabs(p->getInverseMass()) < 1e-10)
		return; 

	p->addForce(gravity * p->getMass()); 
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
