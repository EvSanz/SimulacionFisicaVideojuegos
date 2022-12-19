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

public:

	RigidbodySystem() { forceRegistry = new RigidbodyForceRegistry(); }
	~RigidbodySystem(); 

	virtual void update(double t);

	void addSystem(RigidBodyGenerator* rb) { rigidbodyGenerators.push_back(rb); }
};