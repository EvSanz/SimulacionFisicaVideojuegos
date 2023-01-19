#ifndef __PARTICULAGENERATOR_H__
#define __PARTICULAGENERATOR_H__

#pragma once

#include "PxPhysicsAPI.h"

#include "RenderUtils.hpp"

#include <vector>
#include <string>
#include <list>
#include <random>
#include <complex>

class Particula;
using namespace std; 

class ParticleGenerator
{
protected: 
 
	Vector3 pos, vel, acc; 

	int nParticulas; 

	double gen_prob;

	bool alive = false; 

	std::string name; 

	Particula* modelo = nullptr; 

public: 

	~ParticleGenerator();

	void setParticle(Particula* auxModel) { modelo = auxModel; }

	virtual list<Particula*> generateParticle() = 0; 

	void setAlive() { alive = !alive; };
	bool isAlive() { return alive; };

	Vector3 getPos() { return pos; }
	void changePos(Vector3 position) { pos = position; }

	std::string getName() const { return name; }
	void setName(std::string nombre) { name = nombre; }

	std::default_random_engine gen;
	std::uniform_real_distribution<> distribution{ -1,1 };
	std::normal_distribution<> dist{ 0.5, 0.5 };
};

//////////////////////////////////////////////////////////////////////////////////

class GaussianParticleGenerator : public ParticleGenerator
{
protected:

	bool activoGauss; 
	Vector3 pos_gauss, vel_gauss;
	std::mt19937 random_generator;

public:

	GaussianParticleGenerator(Particula* model, double genProb, Vector3 auxPos, Vector3 auxVel, int particulas);
	std::list<Particula*> generateParticle() override;
};

//////////////////////////////////////////////////////////////////////////////////

class UniformParticleGenerator : public ParticleGenerator
{
protected:

	bool activoUniform; 
	Vector3 uni_pos, uni_vel;
	std::mt19937 random_generator;

public:

	UniformParticleGenerator(Particula* model, double genProb, Vector3 auxPos, Vector3 auxVel, int particulas);
	std::list<Particula*> generateParticle() override;
};

////////////////////////////////////////////////////////////////////////////////

class SphereParticleGenerator : public ParticleGenerator
{
protected:

	bool activoFire; 

	float radio;

	Vector3 fire_pos, fire_vel, fire_acc;

	std::uniform_real_distribution<float> random;

public:

	SphereParticleGenerator(Vector3 pos, Particula* model, float radius, int numParticles);
	std::list<Particula*> generateParticle() override;
};

#endif

