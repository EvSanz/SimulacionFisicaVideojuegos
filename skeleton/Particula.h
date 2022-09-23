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
	Particula(Vector3 posicion, Vector3 velocidad, Vector3 aceleracion, float damping, int disparo);
	~Particula();

	void integrate(double t); 

protected: 

	float damp; 

	Vector3 vel, acc; 

	physx::PxTransform pos; 

	RenderItem* renderItem; 
};

#endif