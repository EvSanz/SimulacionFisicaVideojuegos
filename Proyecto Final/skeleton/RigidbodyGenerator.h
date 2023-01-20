#pragma once

#include "Rigidbody.h"
#include "PxPhysicsAPI.h"

#include "RenderUtils.hpp"

#include <vector>
#include <string>
#include <list>
#include <random>
#include <complex>
#include <chrono>

class RigidBodyGenerator
{
protected:

	std::string name;

	Vector4 color; 
	Vector3 auxPos, auxVel, pos, vel, tam, inertia;
	double masa, vida;
	int nParticulas, forma;

	Rigidbody* model;

	PxScene* scene; 
	PxPhysics* fisicas; 

	std::default_random_engine gen;
	std::uniform_real_distribution<> distribution{ -1,1 };
	std::normal_distribution<> dist{ 0.0, 1.0 };

public:

	~RigidBodyGenerator() {};

	void setModel(Rigidbody* m) { model = m; };

	virtual std::list<Rigidbody*> generateBodies() = 0;
};

//////////////////////////////////////////////////////////////////////////////////

class UniformBodyGenerator : public RigidBodyGenerator
{
protected:

	Vector3 uni_pos, uni_vel;
	std::mt19937 random_generator;

public:

	UniformBodyGenerator(PxScene* scene, PxPhysics* physics, Rigidbody* m, Vector3 auxPos, Vector3 auxVel, int particulas);
	std::list<Rigidbody*> generateBodies() override;
};

/////////////////////////////////////////////////////////////////////////////////

class GaussianBodyGenerator : public RigidBodyGenerator
{
protected:

	Vector3 pos_gauss, vel_gauss;
	std::mt19937 random_generator;

public:

	GaussianBodyGenerator(PxScene* scene, PxPhysics* physics, Rigidbody* m, Vector3 auxPos, Vector3 auxVel, int particulas);
	std::list<Rigidbody*> generateBodies() override;
};