#ifndef __PARTICULAGENERATOR_H__
#define __PARTICULAGENERATOR_H__

#pragma once

#include "PxPhysicsAPI.h"

#include "RenderUtils.hpp"
#include "Particula.h"

#include <vector>
#include <string>
#include <list>
#include <random>
#include <complex>

using namespace std; 

class ParticleGenerator
{
protected: 
 
	Vector3 pos, vel, acc; 

	int nParticulas; 

	double gen_prob;

	bool alive = false; 

	Particula* modelo; 

public: 

	~ParticleGenerator();

	void setParticle(Particula* auxModel) { modelo = auxModel; }

	virtual list<Particula*> generateParticle() = 0; 

	void setAlive() { alive = !alive; };
	bool isAlive() { return alive; };

	std::default_random_engine gen;
	std::uniform_real_distribution<> distribution{ -1,1 };
	std::normal_distribution<> dist{ 0.5, 0.5 };
};

//////////////////////////////////////////////////////////////////////////////////

class GaussianParticleGenerator : public ParticleGenerator
{
protected:

	Vector3 pos_gauss, vel_gauss;
	std::mt19937 random_generator;

public:

	GaussianParticleGenerator(Vector3 auxPos, Vector3 auxVel, int particulas);
	virtual std::list<Particula*> generateParticle() override;
};

//////////////////////////////////////////////////////////////////////////////////

class UniformParticleGenerator : public ParticleGenerator
{
protected:

	Vector3 uni_pos, uni_vel;
	std::mt19937 random_generator;

public:

	UniformParticleGenerator(Vector3 auxPos, Vector3 auxVel, int particulas);
	virtual std::list<Particula*> generateParticle() override;
};

////////////////////////////////////////////////////////////////////////////////

class SphereParticleGenerator : public ParticleGenerator
{
protected:

	float radio;

	Vector3 fire_pos, fire_vel, fire_acc;
	std::uniform_real_distribution<float> random;

public:

	SphereParticleGenerator(Vector3 pos, Particula* model, float radius, int numParticles);
	virtual std::list<Particula*> generateParticle() override;

	std::random_device rd{};
	std::mt19937 gen{ rd() };
};

#endif

