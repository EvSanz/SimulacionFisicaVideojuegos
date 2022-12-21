#pragma once

#include <list>

#include "Rigidbody.h"
#include "RigidbodyGenerator.h"
#include "ParticleForceRegistry.h"
#include "RigidBodyForceGenerator.h"

class RigidbodySystem
{
protected:

	std::list<DinamicRigidbody*> part;
	std::list<RigidBodyGenerator*> rigidbodyGenerators;
	std::list<RigidBodyForceGenerator*> forceGenerators;

	RigidbodyForceRegistry* forceRegistry;

	UniformBodyGenerator* uniform = nullptr; 
	GaussianBodyGenerator* gauss = nullptr;

	bool uniformBodyActive = false, gaussBodyActive = false; 

public:

	RigidbodySystem() { forceRegistry = new RigidbodyForceRegistry(); }
	~RigidbodySystem(); 

	virtual void update(double t);

	void addGenerator(RigidBodyGenerator* rg) { rigidbodyGenerators.push_back(rg); }

	void addUniform(PxPhysics* physics, PxScene* scene, PxMaterial* mat);
	void addGauss(PxPhysics* physics, PxScene* scene, PxMaterial* mat);

	bool isUniformBodyActive() { return uniformBodyActive; }
	void changeUniformBodyState() { uniformBodyActive = !uniformBodyActive; }

	bool isGaussBodyActive() { return gaussBodyActive; }
	void changeGaussBodyState() { gaussBodyActive = !gaussBodyActive; }
};