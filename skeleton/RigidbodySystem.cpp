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
	forceRegistry->clear();
}

void RigidbodySystem::update(double t)
{
	std::list<DinamicRigidbody*> lista;

	for (RigidBodyGenerator* g : rigidbodyGenerators)
	{
		lista = g->generateBodies();

		for (DinamicRigidbody* p : lista)
		{
			part.push_back(p);

			for (RigidBodyForceGenerator* fg : forceGenerators) 
				forceRegistry->addForceRegistry(fg, p);
		}
	}

	std::list<DinamicRigidbody*>::iterator it = part.begin();

	while (it != part.end())
	{
		(*it)->integrate(t);

		if ((*it)->getTimeRigidbody() <= 0.0)
		{
			DinamicRigidbody* p = (*it);
			it = part.erase(it);

			for (DinamicRigidbody* dp : lista)
			{
				part.push_back(dp);

				for (RigidBodyForceGenerator* fg : forceGenerators) 
					forceRegistry->addForceRegistry(fg, dp);
			}

			forceRegistry->deleteForceRegistry(p);
			delete p;
		}

		else 
			it++;
	}

	forceRegistry->updateRigidbodyForces(t);
}

void RigidbodySystem::addUniform(PxPhysics* physics, PxScene* scene, PxTransform trans, PxMaterial* mat, PxGeometry* geo)
{
	DinamicRigidbody* rb = new DinamicRigidbody(physics, scene, trans, mat, geo, {1.0, 0.0, 0.0, 1.0}, 20);

	uniformRB = new UniformBodyGenerator(rb, 0.9, { 5, 0, 5 }, { 5, 0, 0.01 }, 40);

	rigidbodyGenerators.push_back(uniformRB); 
}