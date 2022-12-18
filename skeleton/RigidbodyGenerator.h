#pragma once

#include "Rigidbody.h"

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

	Vector3 auxPos, auxVel, pos, vel;

	double gen_prob;

	int nParticulas;

	Rigidbody* model;

	std::default_random_engine randomGenerator;

public:

	RigidBodyGenerator() {}
	~RigidBodyGenerator() { delete model; }

	void setModel(Rigidbody* m) { model = m; };

	virtual std::list<Rigidbody*> generateBodies() = 0;
};

//////////////////////////////////////////////////////////////////////////////////

class UniformBodyGenerator : public RigidBodyGenerator
{
protected:

	bool activoUniform;
	Vector3 uni_pos, uni_vel;
	std::mt19937 random_generator;

public:

	UniformBodyGenerator(Rigidbody* model, double genProb, Vector3 auxPos, Vector3 auxVel, int particulas);
	std::list<Rigidbody*> generateBodies() override;
};