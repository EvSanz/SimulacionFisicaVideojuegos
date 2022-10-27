#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

#pragma once

#include "RenderUtils.hpp"
#include "core.hpp"

#include "Particula.h"
#include "PxPhysicsAPI.h"
#include "ParticleGenerator.h"
#include "ClasesParticulas.h"

#include <vector>
#include <string>
#include <list>
#include <memory>

class ParticleSystem
{
protected:

	std::list<Particula*> part; 
	std::list<ParticleGenerator*> generadores;

	Vector3 position;

	bool gaussActivo = false;
	bool uniformActivo = false; 

	GaussianParticleGenerator* gaussianGen = nullptr; 
	UniformParticleGenerator* uniformGen = nullptr;

public:

	ParticleSystem(Vector3 pos) { position = pos; }
	~ParticleSystem();

	void update(double t); 

	ParticleGenerator* getParticleGenerator(string name); 

	void generateFireworkSystem(); 
	void generateWaterSystem();
	void generateFogSystem();

	void setFogActive() { gaussActivo = !gaussActivo;}
	bool isFogActive() { return gaussActivo; }

	void setWaterActive() { uniformActivo = !uniformActivo; }
	bool isWaterActive() { return uniformActivo; }
};

#endif
