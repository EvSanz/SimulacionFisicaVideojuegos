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

	//forceRegistry->clear();
}

void RigidbodySystem::update(double t)
{
	std::list<DinamicRigidbody*> lista;

	for (RigidBodyGenerator* g : rigidbodyGenerators)
	{
		lista = g->generateBodies();

		for (DinamicRigidbody* p : lista)
			part.push_back(p);
	}

	for (std::list<DinamicRigidbody*>::iterator it = part.begin(); it != part.end();)
	{
		(*it)->integrate(t);

		if ((*it)->getTimeRigidbody() <= 0.0)
		{
			if ((*it) != nullptr) delete (*it);
			part.erase(it);
		}

		else 
			it++;
	}
}