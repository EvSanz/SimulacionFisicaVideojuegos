#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

#pragma once

#include "RenderUtils.hpp"
#include "core.hpp"

#include "Particula.h"
#include "PxPhysicsAPI.h"
#include "ParticleGenerator.h"
#include "ForceGenerator.h"
#include "ClasesParticulas.h"
#include "ParticleForceRegistry.h"

#include <vector>
#include <string>
#include <list>
#include <memory>
#include <iostream>

class ParticleSystem
{
protected:

	std::list<Particula*> part; 
	std::list<ParticleGenerator*> generadores;

	Vector3 position;

	float timer = 0; 

	GaussianParticleGenerator* gaussianGen = nullptr; 
	UniformParticleGenerator* uniformGen = nullptr;
	SphereParticleGenerator* fireGen = nullptr;
	GravityForceGenerator* gravityGen = nullptr;
	WindForceGenerator* windGen = nullptr; 
	TornadeForceGenerator* tornadeGen = nullptr; 
	ExplosionForceGenerator* explodeGen = nullptr; 
	BungeeForceGenerator* bungee = nullptr; 
	AnchoredSpringFG* muelleAnclado = nullptr; 

	ParticleForceRegistry force;

public:

	ParticleSystem(Vector3 pos) { position = pos; }
	~ParticleSystem();

	void update(double t); 

	ParticleGenerator* getParticleGenerator(string name); 

	void crearSuelo(Vector4 colores, Vector3 pos);

	void generateFireworkSystem(Vector3 pos, Vector4 colores, double masa);
	void generateMuelleAnclado(Vector3 pos);
	void generateFogSystem(Vector3 pos);

	void getPosAvion(Vector3 pos) { position = pos; }

	///////////////////////////////////////////////////////////////////////

	//NO USADOS EN EL PROYECTO

	//ForceGenerator* getForceGenerator(string name);
	void generateGravity();
	void generateMuelle();
	void generateEstela(Vector3 pos);
	void generateBungee(); 
	void generateBounyancy(); 
	void generateElasticBand(); 
	void generateExplosive(int n, int r); 
	void generateStorm(int n, int r); 

	void moreK() { muelleAnclado->setK(5.0); }
	void lessK() { muelleAnclado->setK(-5.0); }

	/*void setForceActive() { fuerzasActivadas = !fuerzasActivadas; }

	void setFogActive() { gaussActivo = !gaussActivo;}
	bool isFogActive() { return gaussActivo; }

	void setWaterActive() { uniformActivo = !uniformActivo; }
	bool isWaterActive() { return uniformActivo; }

	void setFireActive() { fireActivo = !fireActivo; }
	bool isFireActive() { return fireActivo; }*/
};

#endif
