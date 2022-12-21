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
	std::list<DinamicRigidbody*> lista;

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

	/*for (std::list<DinamicRigidbody*>::iterator it = part.begin(); it != part.end();)
	{
		(*it)->integrate(t);

		if (!(*it)->isAlive())
		{
			if ((*it) != nullptr) delete (*it);
			part.erase(it);
		}

		else 
			it++;
	}*/
}

void RigidbodySystem::addUniform(PxPhysics* physics, PxScene* scene, PxMaterial* mat)
{
	PxRigidDynamic* aux = nullptr;

	DinamicRigidbody* rb = new DinamicRigidbody(physics, scene, mat, aux, { 1.0, 1.0, 0.0, 1.0 }, { 0.0, 30.0, 0.0 },
		{ 0.0, 0.0, 0.0 }, { 0.8, 0.8, 0.8 }, 3, 2);

	uniform = new UniformBodyGenerator(rb, 0.9, { 0.0, 50.0, 0.0 }, { 5, 0, 0.01 }, 2);

	addGenerator(uniform);
}

void RigidbodySystem::addGauss(PxPhysics* physics, PxScene* scene, PxMaterial* mat)
{
	PxRigidDynamic* aux = nullptr;

	DinamicRigidbody* rb = new DinamicRigidbody(physics, scene, mat, aux, { 1.0, 0.0, 0.0, 1.0 }, { 0.0, 30.0, 0.0 },
		{ 0.0, 0.0, 0.0 }, { 0.8, 0.8, 0.8 }, 2, 2);

	gauss = new GaussianBodyGenerator(rb, 0.9, { 0.0, 50.0, 0.0 }, { 5, 0, 0.01 }, 2);

	addGenerator(gauss); 
}