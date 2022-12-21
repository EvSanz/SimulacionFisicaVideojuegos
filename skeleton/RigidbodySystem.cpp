#include "RigidbodySystem.h"

RigidbodySystem::~RigidbodySystem()
{
	for (DinamicRigidbody* b : part) 
		delete b;

	for (RigidBodyGenerator* g : rigidbodyGenerators) 
		delete g;

	part.clear();
	rigidbodyGenerators.clear();
	forceGenerators.clear();
}

void RigidbodySystem::update(double t)
{
		if (uniformBodyActive)
		{
			for (auto p : uniform->generateBodies())
				part.push_back(p);
		}

		if (gaussBodyActive)
		{
			for (auto p : gauss->generateBodies())
				part.push_back(p);
		}

		if (sysFuerzas)
			forceRegistry.updateRigidbodyForces(t);

		for (auto it = part.begin(); it != part.end();)
		{
			if ((*it) != nullptr)
			{
				(*it)->integrate(t);

				if (!(*it)->isAlive())
				{
					if (sysFuerzas)
						forceRegistry.deleteForceRegistry(*it);

					delete (*it);
					it = part.erase(it);
				}
				else
					it++;
			}
		}
}

void RigidbodySystem::addUniform(PxPhysics* physics, PxScene* scene, PxMaterial* mat)
{
	PxRigidDynamic* aux = nullptr;

	DinamicRigidbody* rb = new DinamicRigidbody(physics, scene, mat, aux, { 1.0, 1.0, 0.0, 1.0 }, { 0.0, 30.0, 0.0 },
		{ 0.0, 0.0, 0.0 }, { 0.8, 0.8, 0.8 }, 10, 2);
	part.push_back(rb);

	uniform = new UniformBodyGenerator(rb, 0.9, { 0.0, 50.0, 0.0 }, { 5, 0, 0.01 }, 2);
	addGenerator(uniform);
}

void RigidbodySystem::addGauss(PxPhysics* physics, PxScene* scene, PxMaterial* mat)
{
	PxRigidDynamic* aux = nullptr;

	DinamicRigidbody* rb = new DinamicRigidbody(physics, scene, mat, aux, { 1.0, 0.0, 0.0, 1.0 }, { 0.0, 30.0, 0.0 },
		{ 0.0, 0.0, 0.0 }, { 0.8, 0.8, 0.8 }, 10, 2);
	part.push_back(rb); 

	gauss = new GaussianBodyGenerator(rb, 0.9, { 0.0, 50.0, 0.0 }, { 5, 0, 0.01 }, 2);
	addGenerator(gauss); 
}

void RigidbodySystem::createGravity(PxPhysics* physics, PxScene* scene, PxMaterial* mat)
{
	PxRigidDynamic* aux = nullptr;

	gravityGen = new GravityForceRigidbodyGenerator(Vector3(0.0f, -9.8f, 0.0f));

	DinamicRigidbody* p1 = new DinamicRigidbody(physics, scene, mat, aux, { 1.0, 0.0, 0.0, 1.0 }, { 0.0, 50.0, 0.0 },
		{ 0.0, 0.0, 0.0 }, { 0.8, 0.8, 0.8 }, 10, 2);
	forceRegistry.addForceRegistry(gravityGen, p1);
	part.push_back(p1);

	DinamicRigidbody* p2 = new DinamicRigidbody(physics, scene, mat, aux, { 1.0, 0.5, 0.0, 1.0 }, { 10.0, 50.0, 0.0 },
		{ 0.0, 0.0, 0.0 }, { 0.8, 0.8, 0.8 }, 10, 0.6);
	forceRegistry.addForceRegistry(gravityGen, p2);
	part.push_back(p2);

	DinamicRigidbody* p3 = new DinamicRigidbody(physics, scene, mat, aux, { 1.0, 1.0, 0.0, 1.0 }, { -10.0, 50.0, 0.0 },
		{ 0.0, 0.0, 0.0 }, { 0.8, 0.8, 0.8 }, 10, 20);
	forceRegistry.addForceRegistry(gravityGen, p3);
	part.push_back(p3);

	forceGenerators.push_back(gravityGen); 
}

void RigidbodySystem::createWind(PxPhysics* physics, PxScene* scene, PxMaterial* mat, double r, int n)
{
	PxRigidDynamic* aux = nullptr;
	TornadeForceRigidbodyGenerator* tornadeGen = new TornadeForceRigidbodyGenerator(Vector3((float)(n/2), 0.0, 0.0), r, 5.0);

	for (int i = 0; i < n; i++)
	{
		DinamicRigidbody* p1 = new DinamicRigidbody(physics, scene, mat, aux, { 1.0, 0.0, 0.0, 1.0 }, { (float)(i * 2), 50.0, 0.0 },
			{ 0.0, 0.0, 0.0 }, { 0.8, 0.8, 0.8 }, 10, 2);
		forceRegistry.addForceRegistry(tornadeGen, p1);
		part.push_back(p1);
	}

	forceGenerators.push_back(tornadeGen);
}