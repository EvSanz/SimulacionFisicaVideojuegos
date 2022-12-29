#pragma once

#include <list>

#include "Rigidbody.h"
#include "RigidbodyGenerator.h"
#include "ParticleForceRegistry.h"
#include "RigidBodyForceGenerator.h"

class RigidbodySystem
{
protected:

	std::list<Rigidbody*> part;
	std::list<RigidBodyGenerator*> rigidbodyGenerators;
	std::list<RigidBodyForceGenerator*> forceGenerators;

	RigidbodyForceRegistry forceRegistry;

	UniformBodyGenerator* uniform = nullptr; 
	GaussianBodyGenerator* gauss = nullptr;

	GravityForceRigidbodyGenerator* gravityGen;
	WindForceRigidbodyGenerator* windGen;
	ExplosionForceRigidbodyGenerator* expGen; 

	Vector3 position = {0.0, 0.0, 0.0};

	bool uniformBodyActive = false, gaussBodyActive = false, sysFuerzas = true; 

public:

	RigidbodySystem(Vector3 pos) { position = pos; }
	~RigidbodySystem(); 

	virtual void update(double t);

	void addGenerator(RigidBodyGenerator* rg) { rigidbodyGenerators.push_back(rg); }

	void addUniform(PxPhysics* physics, PxScene* scene, PxMaterial* mat);
	void addGauss(PxPhysics* physics, PxScene* scene, PxMaterial* mat);

	void createGravity(PxPhysics* physics, PxScene* scene, PxMaterial* mat);
	void createWind(PxPhysics* physics, PxScene* scene, PxMaterial* mat, double r, int n);
	void createExplosive(PxPhysics* physics, PxScene* scene, PxMaterial* mat, int n, int r); 

	bool isUniformBodyActive() { return uniformBodyActive; }
	void changeUniformBodyState() { uniformBodyActive = !uniformBodyActive; }

	bool isGaussBodyActive() { return gaussBodyActive; }
	void changeGaussBodyState() { gaussBodyActive = !gaussBodyActive; }

	void setForceActive() { sysFuerzas = !sysFuerzas; }
};