#pragma once

#include "Rigidbody.h"

#include <vector>
#include <string>
#include <list>
#include <random>
#include <complex>

class RigidBodyGenerator
{
protected:

	std::string name;

	Vector3 auxPos, auxVel, pos, vel;

	double gen_prob;

	int nParticulas;

	DinamicRigidbody* model;

	std::default_random_engine randomGenerator;

public:

	RigidBodyGenerator() {}
	~RigidBodyGenerator() { delete model; }

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