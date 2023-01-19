#include "RigidbodySystem.h"

RigidbodySystem::~RigidbodySystem()
{
	for (Rigidbody* b : part)
		delete b;

	for (Rigidbody* b : partEstaticos)
		delete b;


	for (RigidBodyGenerator* g : rigidbodyGenerators) 
		delete g;

	part.clear();
	partEstaticos.clear(); 

	rigidbodyGenerators.clear();
	forceGenerators.clear();
}

void RigidbodySystem::update(double t)
{
	forceRegistry.updateRigidbodyForces(t);

	for (auto it = part.begin(); it != part.end();)
	{
		(*it)->integrate(t);

		if (!(*it)->isAlive() || (*it)->getPosition().x + 100.0 < position.x)
		{
			forceRegistry.deleteForceRegistry(*it);

			delete (*it);
			it = part.erase(it);
		}
		else
			it++;
	}

	for (auto it = partEstaticos.begin(); it != partEstaticos.end();)
	{
		if (!(*it)->isAlive() || (*it)->getPosition().x + 100.0 < position.x)
		{
			delete (*it);
			it = partEstaticos.erase(it);
		}
		else
			it++;
	}
}

void RigidbodySystem::createMuelleAnclado(PxPhysics* physics, PxScene* scene, Vector3 pos)
{
	gravityGen = new GravityForceRigidbodyGenerator(Vector3(0.0f, -6.0f, 0.0f));

	Rigidbody* p = new Rigidbody(scene, physics, /*pos*/pos, /*vel*/{ 0.0, 0.0, 0.0 }, /*size*/{ 3.0, 0.8, 0.8 },
		/*mass*/2, /*time*/10, /*color*/{ 1.0, 0.0, 0.0, 1.0 }, /*dinamic?*/true, /*shape*/1, "globo");
	p->notAllowedToDie(); 

	int resting = rand() % 10 + 20; 
	int k = rand() % 30 + 60; 

	AnchoredSpringRigidbodyGenerator* muelle = new AnchoredSpringRigidbodyGenerator(scene, physics, k, resting, { pos.x, 110.0, 0.0 });

	forceRegistry.addForceRegistry(muelle, p);
	forceRegistry.addForceRegistry(gravityGen, p);

	forceGenerators.push_back(muelle);
	part.push_back(p);

}

void RigidbodySystem::destruirRigido(PxActor* obj)
{
	PxActor* act;
	Rigidbody* p1 = nullptr;

	auto i = part.begin();

	while (p1 == nullptr && i != part.end())
	{
		act = (*i)->getDinamico();

		if (obj == act)
		{
			p1 = (*i);
			(*i)->killRigidbody();
		}

		else
			++i;
	}
}

void RigidbodySystem::destruirRigidoEstatico(PxActor* obj)
{
	PxActor* act;
	Rigidbody* p1 = nullptr;

	auto i = partEstaticos.begin();

	while (p1 == nullptr && i != partEstaticos.end())
	{
		act = (*i)->getEstatico();

		if (obj == act)
		{
			p1 = (*i);
			(*i)->killRigidbody();
		}

		else
			++i;
	}
}

void RigidbodySystem::createTree(PxPhysics* gPhysics, PxScene* gScene, float posX)
{
	int tam = (rand() % 30) + 20.0;

	Rigidbody* tronco = new Rigidbody(gScene, gPhysics, { posX, (float)tam / 2, 0.0 }, { 0.0, 0.0, 0.0 },
		{ 2.0, (float)tam, 2.0 }, 50.0, 50.0, { 1.0, 0.8, 0.8, 1.0 }, false, 1, "indestructible");
	partEstaticos.push_back(tronco);

	int nCopas = (rand() % 3) + 1;

	for (int i = 0; i < nCopas; i++)
	{
		Rigidbody* copa = new Rigidbody(gScene, gPhysics, { posX, (float)(tam + tam / 2 * i), 0.0 }, { 0.0, 0.0, 0.0 },
			{ (float)(10.0 - 2.0 * i), 6.0, 10.0 }, 50.0, 50.0, { 0.0, 1.0, 0.0, 1.0 }, false, 1, "indestructible");
		partEstaticos.push_back(copa);
	}
}

void RigidbodySystem::createBullet(PxPhysics* gPhysics, PxScene* gScene, Vector3 pos)
{	
	if (bulletType == 0)
		part.push_back(new Rigidbody(gScene, gPhysics, pos, { 100.0, 0.0, 0.0 },
		{ 0.8, 0.8, 0.8 }, 10.0, 10.0, { 1.0, 1.0, 0.0, 1.0 }, true, 1, "bala"));
	else if (bulletType == 1)
		part.push_back(new Rigidbody(gScene, gPhysics, pos + Vector3(0.5, 0.0, 0.0), {70.0, 0.0, 0.0},
		{ 1.0, 1.0, 1.0 }, 20.0, 20.0, { 0.0, 0.0, 0.0, 1.0 }, true, 1, "bala"));
	else
		part.push_back(new Rigidbody(gScene, gPhysics, pos, { 150.0, 0.0, 0.0 },
		{ 0.5, 0.5, 0.5 }, 5.0, 10.0, { 0.0, 1.0, 0.0, 1.0 }, true, 1, "bala"));
}

//////////////////////////////////////////////////////////////////////////////////////

//NO USADOS EN EL PROYECTO

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

//void RigidbodySystem::addGauss(PxPhysics* physics, PxScene* scene, PxMaterial* mat)
//{
//	Rigidbody* rb = new Rigidbody(scene, physics, /*pos*/{ 0.0, 50.0, 0.0 }, /*vel*/{ 0.0, 0.0, 0.0 }, /*size*/{ 0.8, 0.8, 0.8 },
//		/*mass*/2, /*time*/10, /*color*/{ 1.0, 0.0, 0.0, 1.0 }, /*dinamic?*/true, /*shape*/1, " ");
//	part.push_back(rb);
//
//	gauss = new GaussianBodyGenerator(scene, physics, rb, { 0.0, 50.0, 0.0 }, { 0.0, 0.0, 0.0 }, 2);
//	addGenerator(gauss);
//}
//
//void RigidbodySystem::createGravity(PxPhysics* physics, PxScene* scene, PxMaterial* mat)
//{
//	PxRigidDynamic* aux = nullptr;
//
//	gravityGen = new GravityForceRigidbodyGenerator(Vector3(0.0f, -9.8f, 0.0f));
//
//	Rigidbody* p1 = new Rigidbody(scene, physics, /*pos*/{ 0.0, 50.0, 0.0 }, /*vel*/{ 0.0, 0.0, 0.0 }, /*size*/{ 0.8, 0.8, 0.8 },
//		/*mass*/2, /*time*/10, /*color*/{ 1.0, 0.0, 0.0, 1.0 }, /*dinamic?*/true, /*shape*/1, " ");
//	forceRegistry.addForceRegistry(gravityGen, p1);
//	part.push_back(p1);
//
//	Rigidbody* p2 = new Rigidbody(scene, physics, /*pos*/{ 10.0, 50.0, 0.0 }, /*vel*/{ 0.0, 0.0, 0.0 }, /*size*/{ 0.8, 0.8, 0.8 },
//		/*mass*/2, /*time*/10, /*color*/{ 1.0, 0.0, 0.0, 1.0 }, /*dinamic?*/true, /*shape*/0, " ");
//	forceRegistry.addForceRegistry(gravityGen, p2);
//	part.push_back(p2);
//
//	Rigidbody* p3 = new Rigidbody(scene, physics, /*pos*/{ 20.0, 50.0, 0.0 }, /*vel*/{ 0.0, 0.0, 0.0 }, /*size*/{ 0.8, 0.8, 0.8 },
//		/*mass*/2, /*time*/10, /*color*/{ 1.0, 0.0, 0.0, 1.0 }, /*dinamic?*/true, /*shape*/1, " ");
//	forceRegistry.addForceRegistry(gravityGen, p3);
//	part.push_back(p3);
//
//	forceGenerators.push_back(gravityGen);
//}
//
//void RigidbodySystem::createWind(PxPhysics* physics, PxScene* scene, PxMaterial* mat, double r, int n)
//{
//	windGen = new WindForceRigidbodyGenerator(Vector3(100.0, 40.0, 0.0), Vector3(0.0, 10.0, 0.0), r * 2);
//
//	Particula* guiaGen = new Particula(PruebaLineas(Vector3(0.0, 10.0, 0.0), (float)r * 2));
//
//	for (int i = 0; i < n; i++)
//	{
//		Rigidbody* p1 = new Rigidbody(scene, physics, /*pos*/{ (float)(i * 2), 50.0, 0.0 }, /*vel*/{ 0.0, 0.0, 0.0 }, /*size*/{ 0.8, 0.8, 0.8 },
//			/*mass*/2, /*time*/10, /*color*/{ 1.0, 0.0, 0.0, 1.0 }, /*dinamic?*/true, /*shape*/1, " ");
//		forceRegistry.addForceRegistry(windGen, p1);
//		part.push_back(p1);
//	}
//
//	forceGenerators.push_back(windGen);
//}

//void RigidbodySystem::createExplosive(PxPhysics* physics, PxScene* scene, PxMaterial* mat, int n, int r)
//{
//	expGen = new ExplosionForceRigidbodyGenerator(Vector3(30.0, 20.0, 0.0), r * 2, 100);
//	//expGen = new ExplosionForceRigidbodyGenerator(Vector3((float) n / 2, 0.0, (float) n / 2), r * 2, 10);
//
//	Particula* guiaGen = new Particula(PruebaLineas({ 30.0, 20.0, 0.0 }, (float)r));
//
//	for (int i = 0; i < n; i++)
//	{
//		Rigidbody* p = new Rigidbody(scene, physics, /*pos*/{ (float)(15.0 + i * 2), 100.0, 0.0 }, /*vel*/{ 0.0, 0.0, 0.0 }, /*size*/{ 0.8, 0.8, 0.8 },
//			/*mass*/2, /*time*/10, /*color*/{ 1.0, 1.0, 0.0, 1.0 }, /*dinamic?*/true, /*shape*/1, " ");
//
//		forceRegistry.addForceRegistry(expGen, p);
//		part.push_back(p);
//	}
//}