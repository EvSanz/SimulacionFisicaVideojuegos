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

	std::vector<Particula*> part; 
	std::list<ParticleGenerator*> generadores;

	Vector3 position;

public:

	ParticleSystem(Vector3 pos) { position = pos; }
	~ParticleSystem();

	void update(double t); 

	ParticleGenerator* getParticleGenerator(string name); 

	void generateFireworkSystem(); 
	void generateWaterSystem();
	void generateFogSystem();
};

#endif
