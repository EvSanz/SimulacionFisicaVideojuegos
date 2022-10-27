#ifndef __PARTICULA_H__
#define __PARTICULA_H__

#include "PxPhysicsAPI.h"
#include "ClasesParticulas.h"
#include "ParticleGenerator.h"

#include "RenderUtils.hpp"
#include "core.hpp"

#include <vector>
#include <list>
#include <memory>

#pragma once

//PARTICULA

class Particula
{
public: 

	Particula(ClasesParticulas particula);
	~Particula();

	void integrate(double t); 

	void killParticle() { alive = false; }
	bool isAlive() { return alive; }

	bool update(double t); 

	void setVelocity(Vector3 v) { vel = v; }
	void setAcceleration(Vector3 a) { acc = a; }
	void setPosition(Vector3 p) { pos = physx::PxTransform(p.x, p.y, p.z); }

	Vector3 getPos() { return { pos.p.x, pos.p.y, pos.p.z }; }
	Vector3 getVel() { return vel; }
	Vector3 getAcceleration() { return acc; }
	Vector3 getSize() { return size; }

	Vector4 getColor() { return renderItem->color; }

	ClasesParticulas getClass() { return tipoClase; }

protected: 

	double damp, mass, time; 

	bool alive = true; 

	Vector3 vel, acc, position, size; 

	Vector4 color;

	physx::PxTransform pos; 

	RenderItem* renderItem; 

	ClasesParticulas tipoClase; 
};

//////////////////////////////////////////////////////////////////////////

//FIREWORK

class Firework : public Particula 
{
	protected:

		int nFuegos, disparo;

		std::list<std::shared_ptr<ParticleGenerator>> _gens;

	public:

		Firework(ClasesParticulas p,
			std::list<std::shared_ptr<ParticleGenerator>> gens) : Particula(p), _gens(gens) {}

		std::list<Particula*> explode()
		{
			std::list<Particula*> lista;

			for (auto g : _gens) 
			{
				for (auto p : g->generateParticle())
					lista.push_back(p);
			}

			return lista;
		}
};

#endif