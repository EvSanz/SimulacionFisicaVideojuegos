#include <map>
#include <iostream>

#include "ForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(Vector3& gravedad)
{
	gravity = gravedad; 
}

void GravityForceGenerator::updateForce(Particula* p, double t)
{
	if (p->getInverseMass() < 1e-10)
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

	/*Vector3 v = p->getVel() - velViento;
	float coef = 0.003 * v.normalize() + 0.002 * pow(v.normalize(), 2); 
	p->addForce(-v * coef); */

	Vector3 v = velViento - p->getVel(); 
	p->addForce(0.2 * v + 0.001 * v.magnitude() * v);
}

///////////////////////////////////////////////////////////////////

TornadeForceGenerator::TornadeForceGenerator(const Vector3& pos, double r, double aux) :
	WindForceGenerator(Vector3(0, 0, 0), pos, r)
{
	k = aux; 
	centro = pos; 
}

void TornadeForceGenerator::updateForce(Particula* p, double t)
{
	Vector3 d = p->getPos() - centro; 

	velViento = k * Vector3(-(d.z), (20 - d.y), d.x);

	WindForceGenerator::updateForce(p, t); 
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

	p->addForce((8000 / pow(rExplosion, 2)) * (p->getPos() - expPos) * pow(2.7, -((t / 1000.0))));
}

///////////////////////////////////////////////////////////////////

SpringForceGenerator::SpringForceGenerator(Particula* partAux, double auxK, double auxRestLenght)
{
	restLenght = auxRestLenght;
	auxiliar = partAux; 
	k = auxK;  
}

void SpringForceGenerator::updateForce(Particula* p, double t)
{
	Vector3 force = auxiliar->getPos() - p->getPos(); 

	const float lenght = force.normalize(); 
	const float deltaX = lenght - restLenght; 

	force *= deltaX * k; 

	p->addForce(force); 
}

///////////////////////////////////////////////////////////////////

AnchoredSpringFG::AnchoredSpringFG(double k, double resting, const Vector3& anchor_pos) : 
	SpringForceGenerator(nullptr, k, resting)
{
	auxiliar = new Particula(CajaMuelle({1.0, 0.0, 0.0, 1.0}, anchor_pos, 100));
}

///////////////////////////////////////////////////////////////////

ElasticForce::ElasticForce(double k, double resting, Particula* p) : SpringForceGenerator(p, k, resting) {}

///////////////////////////////////////////////////////////////////

BungeeForceGenerator::BungeeForceGenerator(float h, float V, float d)
{
	altura = h; 
	volumen = V; 
	densidadLiquido = d; 

	particulaLiquido = new Particula(CajaMuelle({ 0.0, 0.0, 0.0, 1.0 }, { 0.0, h, 0.0 }, 30));
}

void BungeeForceGenerator::updateForce(Particula* p, double t)
{
	float immersed = 0.0; 
	float h = p->getPos().y; 
	float h0 = particulaLiquido->getPos().y; 

	Vector3 f(0, 0, 0); 

	if (h - h0 > altura + 0.5)
		immersed = 0.0;
	else if (h0 - h > altura + 0.5)
		immersed = 1.0;
	else
		immersed = (h0 - h) / (altura + 0.5); 

	f.y = densidadLiquido * volumen * immersed * gravedad;

	p->addForce(f); 
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
