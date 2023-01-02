#include "RigidbodySystem.h"

RigidbodySystem::~RigidbodySystem()
{
	for (Rigidbody* b : part) 
		delete b;

	for (RigidBodyGenerator* g : rigidbodyGenerators) 
		delete g;

	part.clear();
	rigidbodyGenerators.clear();
	forceGenerators.clear();
}

void RigidbodySystem::update(double t)
{
		/*if (uniformBodyActive)
		{
			for (auto p : uniform->generateBodies())
				part.push_back(p);
		}

		if (gaussBodyActive)
		{
			for (auto p : gauss->generateBodies())
				part.push_back(p);
		}

		if (sysFuerzas)*/

		forceRegistry.updateRigidbodyForces(t);

		for (auto it = part.begin(); it != part.end();)
		{
			if ((*it) != nullptr)
			{
				(*it)->integrate(t);

				if (!(*it)->isAlive()) 
				{
					//if (sysFuerzas)
					forceRegistry.deleteForceRegistry(*it);

					delete (*it);
					it = part.erase(it);
				}
				else
					it++;
			}
		}
}

//void RigidbodySystem::addUniform(PxPhysics* physics, PxScene* scene, PxMaterial* mat)
//{
//	PxRigidDynamic* aux = nullptr;
//
//	Rigidbody* rb = new Rigidbody(scene, physics, /*pos*/{ 0.0, 50.0, 0.0 }, /*vel*/{ 0.0, 0.0, 0.0 }, /*size*/{ 0.8, 0.8, 0.8 },
//		/*mass*/2, /*time*/10, /*color*/{ 1.0, 0.0, 0.0, 1.0 }, /*dinamic?*/true, /*shape*/1, " ");
//	part.push_back(rb);
//
//	uniform = new UniformBodyGenerator(scene, physics, rb, { 0.0, 50.0, 0.0 }, { 0.0, 0.0, 0.0 }, 2);
//	addGenerator(uniform);
//}

void RigidbodySystem::addGauss(PxPhysics* physics, PxScene* scene, PxMaterial* mat)
{
	Rigidbody* rb = new Rigidbody(scene, physics, /*pos*/{ 0.0, 50.0, 0.0 }, /*vel*/{ 0.0, 0.0, 0.0 }, /*size*/{ 0.8, 0.8, 0.8 },
		/*mass*/2, /*time*/10, /*color*/{ 1.0, 0.0, 0.0, 1.0 }, /*dinamic?*/true, /*shape*/1, " ");
	part.push_back(rb); 

	gauss = new GaussianBodyGenerator(scene, physics, rb, {0.0, 50.0, 0.0}, {0.0, 0.0, 0.0}, 2);
	addGenerator(gauss); 
}

void RigidbodySystem::createGravity(PxPhysics* physics, PxScene* scene, PxMaterial* mat)
{
	PxRigidDynamic* aux = nullptr;

	gravityGen = new GravityForceRigidbodyGenerator(Vector3(0.0f, -9.8f, 0.0f));

	Rigidbody* p1 = new Rigidbody(scene, physics, /*pos*/{ 0.0, 50.0, 0.0 }, /*vel*/{ 0.0, 0.0, 0.0 }, /*size*/{ 0.8, 0.8, 0.8 },
		/*mass*/2, /*time*/10, /*color*/{ 1.0, 0.0, 0.0, 1.0 }, /*dinamic?*/true, /*shape*/1, " ");
	forceRegistry.addForceRegistry(gravityGen, p1);
	part.push_back(p1);

	Rigidbody* p2 = new Rigidbody(scene, physics, /*pos*/{ 10.0, 50.0, 0.0 }, /*vel*/{ 0.0, 0.0, 0.0 }, /*size*/{ 0.8, 0.8, 0.8 },
		/*mass*/2, /*time*/10, /*color*/{ 1.0, 0.0, 0.0, 1.0 }, /*dinamic?*/true, /*shape*/0, " ");
	forceRegistry.addForceRegistry(gravityGen, p2);
	part.push_back(p2);

	Rigidbody* p3 = new Rigidbody(scene, physics, /*pos*/{ 20.0, 50.0, 0.0 }, /*vel*/{ 0.0, 0.0, 0.0 }, /*size*/{ 0.8, 0.8, 0.8 },
		/*mass*/2, /*time*/10, /*color*/{ 1.0, 0.0, 0.0, 1.0 }, /*dinamic?*/true, /*shape*/1, " ");
	forceRegistry.addForceRegistry(gravityGen, p3);
	part.push_back(p3);

	forceGenerators.push_back(gravityGen); 
}

void RigidbodySystem::createWind(PxPhysics* physics, PxScene* scene, PxMaterial* mat, double r, int n)
{
	windGen = new WindForceRigidbodyGenerator(Vector3(100.0, 40.0, 0.0), Vector3(0.0, 10.0, 0.0), r * 2);

	Particula* guiaGen = new Particula(PruebaLineas(Vector3(0.0, 10.0, 0.0), (float)r * 2));

	for (int i = 0; i < n; i++)
	{
		Rigidbody* p1 = new Rigidbody(scene, physics, /*pos*/{(float)(i * 2), 50.0, 0.0}, /*vel*/{0.0, 0.0, 0.0}, /*size*/{0.8, 0.8, 0.8},
			/*mass*/2, /*time*/10, /*color*/{1.0, 0.0, 0.0, 1.0}, /*dinamic?*/true, /*shape*/1, " ");
		forceRegistry.addForceRegistry(windGen, p1);
		part.push_back(p1);
	}

	forceGenerators.push_back(windGen);
}

void RigidbodySystem::createExplosive(PxPhysics* physics, PxScene* scene, PxMaterial* mat, int n, int r)
{
	expGen = new ExplosionForceRigidbodyGenerator(Vector3(30.0, 20.0, 0.0), r * 2, 100);
	//expGen = new ExplosionForceRigidbodyGenerator(Vector3((float) n / 2, 0.0, (float) n / 2), r * 2, 10);

	Particula* guiaGen = new Particula(PruebaLineas({ 30.0, 20.0, 0.0 }, (float)r));

	for (int i = 0; i < n; i++)
	{
		Rigidbody* p = new Rigidbody(scene, physics, /*pos*/{ (float)(15.0 + i * 2), 100.0, 0.0 }, /*vel*/{ 0.0, 0.0, 0.0 }, /*size*/{ 0.8, 0.8, 0.8 },
			/*mass*/2, /*time*/10, /*color*/{ 1.0, 1.0, 0.0, 1.0 }, /*dinamic?*/true, /*shape*/1, " ");

		forceRegistry.addForceRegistry(expGen, p);
		part.push_back(p);
	}
}

void RigidbodySystem::createMuelleAnclado(PxPhysics* physics, PxScene* scene, Vector3 pos)
{
	Rigidbody* p = new Rigidbody(scene, physics, /*pos*/pos, /*vel*/{ 0.0, 0.0, 0.0 }, /*size*/{ 3.0, 0.8, 0.8 },
		/*mass*/2, /*time*/10, /*color*/{ 1.0, 0.0, 0.0, 1.0 }, /*dinamic?*/true, /*shape*/1, "globo");
	p->notAllowedToDie(); 
	AnchoredSpringRigidbodyGenerator* muelle = new AnchoredSpringRigidbodyGenerator(scene, physics, 50, 10, { pos.x, 90.0, 0.0 });

	forceRegistry.addForceRegistry(muelle, p);
	part.push_back(p);
}

//////////////////////////////////////////////////////////////////////////////////////

void RigidbodySystem::destruirGlobo(PxActor* globo)
{
	PxActor* act;
	Rigidbody* p1 = nullptr;

	auto i = part.begin();

	while (p1 == nullptr && i != part.end())
	{
		act = (*i)->getDinamico();

		if (globo == act)
		{
			p1 = (*i);
			(*i)->killRigidbody();
		}

		else
			++i;
	}
}