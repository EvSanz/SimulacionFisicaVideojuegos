#ifndef __PARTICULA_H__
#define __PARTICULA_H__

#include "PxPhysicsAPI.h"

#include "RenderUtils.hpp"
#include "core.hpp"

#include <vector>

#pragma once
class Particula
{
public: 
	Particula(Vector3 posicion, Vector3 velocidad, Vector3 aceleracion, float damping, float mass, int disparo, Vector4 color);
	~Particula();

	void integrate(double t); 

	void killParticle() { alive = false; }
	bool isAlive() { return alive; }

protected: 

	float damp, masa, time; 

	bool alive = true; 

	Vector3 vel, acc, position; 

	physx::PxTransform pos; 

	RenderItem* renderItem; 
};

#endif