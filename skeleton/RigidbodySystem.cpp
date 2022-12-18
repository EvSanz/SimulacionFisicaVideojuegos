#include "RigidbodySystem.h"

RigidbodySystem::~RigidbodySystem()
{
	forceRegistry->clear();

	for (Rigidbody* b : bodies) delete b;
	bodies.clear();

	for (RigidBodyGenerator* g : rigidbodyGenerators) delete g;
	rigidbodyGenerators.clear();

	forceGenerators.clear();
	forceRegistry->clear();
}

void RigidbodySystem::update(double t)
{
	std::list<Rigidbody*> lista;

	for (RigidBodyGenerator* g : rigidbodyGenerators)
	{
		lista = g->generateBodies();

		for (Rigidbody* p : lista)
		{
			bodies.push_back(p);

			for (RigidBodyForceGenerator* fg : forceGenerators) 
				forceRegistry->addForceRegistry(fg, p);
		}
	}

	std::list<Rigidbody*>::iterator it = bodies.begin();

	while (it != bodies.end())
	{
		(*it)->integrate(t);

		if ((*it)->getTimeRigidbody() <= 0.0)
		{
			std::list<Rigidbody*> lista = (*it)->onDeath();

			Rigidbody* p = (*it);
			it = bodies.erase(it);

			for (Rigidbody* np : lista)
			{
				bodies.push_back(np);

				for (RigidBodyForceGenerator* fg : forceGenerators) 
					forceRegistry->addForceRegistry(fg, np);
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