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
	std::list<Rigidbody*> partEstaticos;

	std::list<RigidBodyGenerator*> rigidbodyGenerators;
	std::list<RigidBodyForceGenerator*> forceGenerators;

	RigidbodyForceRegistry forceRegistry;

	UniformBodyGenerator* uniform = nullptr; 
	GaussianBodyGenerator* gauss = nullptr;

	GravityForceRigidbodyGenerator* gravityGen;
	WindForceRigidbodyGenerator* windGen;
	ExplosionForceRigidbodyGenerator* expGen; 

	Vector3 position;

	//bool uniformBodyActive = false, gaussBodyActive = false, sysFuerzas = true; 

public:

	RigidbodySystem(Vector3 pos) { position = pos; }
	~RigidbodySystem(); 

	virtual void update(double t);

	void addGenerator(RigidBodyGenerator* rg) { rigidbodyGenerators.push_back(rg); }

	void getPosAvion(Vector3 pos) { position = pos; }

	void createTree(PxPhysics* gPhysics, PxScene* gScene, float posX);
	void createBullet(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos);

	//void addUniform(PxPhysics* physics, PxScene* scene, PxMaterial* mat);
	void addGauss(PxPhysics* physics, PxScene* scene, PxMaterial* mat);

	void createGravity(PxPhysics* physics, PxScene* scene, PxMaterial* mat);
	void createWind(PxPhysics* physics, PxScene* scene, PxMaterial* mat, double r, int n);
	void createExplosive(PxPhysics* physics, PxScene* scene, PxMaterial* mat, int n, int r); 
	void createMuelleAnclado(PxPhysics* physics, PxScene* scene, Vector3 pos); 

	//////////////////////////////////////////////////////////////////////////////////////////

	void destruirRigido(PxActor* obj);
	void destruirRigidoEstatico(PxActor* obj); 
};