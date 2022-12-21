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
	//DinamicRigidbody(gPhysics, gScene, mat, part);

	Vector4 color; 
	Vector3 auxPos, auxVel, pos, vel, tam;
	double gen_prob, masa, vida;
	int nParticulas;

	DinamicRigidbody* model;

	PxScene* scene; 
	PxMaterial* mat; 
	PxPhysics* fisicas; 
	PxRigidDynamic* rd; 

	std::default_random_engine gen;
	std::uniform_real_distribution<> distribution{ -1,1 };
	std::normal_distribution<> dist{ 0.0, 1.0 };

public:

	~RigidBodyGenerator() {};

	void setModel(DinamicRigidbody* m) { model = m; };

	virtual std::list<DinamicRigidbody*> generateBodies() = 0;
};

//////////////////////////////////////////////////////////////////////////////////

class UniformBodyGenerator : public RigidBodyGenerator
{
protected:

	bool activoUniform;
	Vector3 uni_pos, uni_vel;
	std::mt19937 random_generator;

public:

	UniformBodyGenerator(DinamicRigidbody* model, double genProb, Vector3 auxPos, Vector3 auxVel, int particulas);
	std::list<DinamicRigidbody*> generateBodies() override;
};

/////////////////////////////////////////////////////////////////////////////////

class GaussianBodyGenerator : public RigidBodyGenerator
{
protected:

	bool activoGauss;
	Vector3 pos_gauss, vel_gauss;
	std::mt19937 random_generator;

public:

	GaussianBodyGenerator(DinamicRigidbody* model, double genProb, Vector3 auxPos, Vector3 auxVel, int particulas);
	std::list<DinamicRigidbody*> generateBodies() override;
};