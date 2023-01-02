#include "GameSystem.h"

GameSystem::~GameSystem()
{
	for (Rigidbody* b : arboles)
		delete b;

	for (Rigidbody* b : balas)
		delete b;

	for (Zeppelin* b : naves)
		delete b;

	for (RigidBodyGenerator* g : rigidbodyGenerators)
		delete g;

	balas.clear();
	naves.clear();
	arboles.clear();

	rigidbodyGenerators.clear();
	forceGenerators.clear();

	delete particleSystem;
}

void GameSystem::update(double t)
{
	particleSystem->update(t);
	rigidbodySystem->update(t);

	forceRegistry.updateRigidbodyForces(t);

	if (!noPlane)
	{
		tiempo += t;
		if (tiempoCooldown < tiempo && contadorBalas < cargadorBalas)
		{
			addUIElement({ float((plane->getPos().x + 85.0) - 5.0 * contadorBalas), 105.0, 0.0 }, true);

			contadorBalas++;
			tiempo = 0.0;
		}

		for (auto it = UIBalas.begin(); it != UIBalas.end(); it++)
		{
			Vector3 position = { float((*it)->getPos().x + 0.01), 105.0, 0.0 };
			(*it)->setPosition(position);
		}

		for (auto it = UIVidas.begin(); it != UIVidas.end(); it++)
		{
			Vector3 position = { float((*it)->getPos().x + 0.01), 105.0, 0.0 };
			(*it)->setPosition(position);
		}

		plane->update(t);
	}

	for (auto it = naves.begin(); it != naves.end();)
	{
		(*it)->update(t);

		if (!noPlane)
		{
			if ((*it)->getPos().x + 50.0 < plane->getPos().x 
			|| !(*it)->getRigidbody()->isAlive())
			{
				delete (*it);
				it = naves.erase(it);
			}

			else
				it++;
		}

		else
		{
			if ((*it)->getPos().x + 50.0 < posAvion
			|| !(*it)->getRigidbody()->isAlive())
			{
				delete (*it);
				it = naves.erase(it);
			}

			else
				it++;
		}
	}

	for (auto it = balas.begin(); it != balas.end();)
	{
		(*it)->integrate(t);

		if (!(*it)->isAlive())
		{
			delete (*it);
			it = balas.erase(it);
		}

		else
			it++;
	}

	for (auto it = arboles.begin(); it != arboles.end();)
	{
		if (!noPlane)
		{
			if ((*it)->getPosition().x + 50.0 < plane->getPos().x)
			{
				delete (*it);
				it = arboles.erase(it);
			}

			else
				it++;
		}

		else
		{
			if ((*it)->getPosition().x + 50.0 < posAvion)
			{
				delete (*it);
				it = arboles.erase(it);
			}

			else
				it++;
		}
	}

	for (auto it = floor.begin(); it != floor.end();)
	{
		if (!noPlane)
		{
			if ((*it)->getPos().x + 100.0 < plane->getPos().x)
			{
				delete (*it);
				it = floor.erase(it);
			}

			else
				it++;
		}

		else
		{
			if ((*it)->getPos().x + 50.0 < posAvion)
			{
				delete (*it);
				it = floor.erase(it);
			}

			else
				it++;
		}
	}
}

void GameSystem::addObstacles(int obstaculo, bool zep)
{
	if (obstaculo == 0 || obstaculo == 1 ||
		obstaculo == 2 || obstaculo == 3 ||
		obstaculo == 4 || obstaculo == 5)
	{
		int tam = (rand() % 30) + 20.0;

		Rigidbody* tronco = new Rigidbody(gScene, gPhysics, { posX, (float)tam / 2, 0.0 }, { 0.0, 0.0, 0.0 },
			{ 2.0, (float)tam, 2.0 }, 50.0, 50.0, { 1.0, 0.8, 0.8, 1.0 }, false, 1, "indestructible");
		arboles.push_back(tronco);

		int nCopas = (rand() % 3) + 1; 

		for (int i = 0; i < nCopas; i++)
		{
			Rigidbody* copa = new Rigidbody(gScene, gPhysics, { posX, (float)(tam + tam / 2 + 6.0 * i), 0.0 }, { 0.0, 0.0, 0.0 },
				{ (float)(10.0 - 2.0 * i), 6.0, 10.0 }, 50.0, 50.0, { 0.0, 1.0, 0.0, 1.0 }, false, 1, "indestructible");
			arboles.push_back(copa);
		}
	}

	else if (obstaculo == 6 || obstaculo == 7 ||
		obstaculo == 8)
	{
		int alt = (rand() % 30) + 50;
		rigidbodySystem->createMuelleAnclado(gPhysics, gScene, { posX, (float)alt, 0.0 }); 
	}

	if (zep)
	{
		int tam = rand() % 30;
		int alt = (rand() % 40) + 60;

		Zeppelin* z = new Zeppelin(gScene, gPhysics, {posX, (float)alt, 0.0});
		naves.push_back(z); 
	}

	posX += 60.0;
}

void GameSystem::shootBullets()
{
	if (contadorBalas > 0)
	{
		Vector3 pos = plane->getPos() + Vector3(9.0f, 0.0f, 0.0f);

		balas.push_back(new Rigidbody(gScene, gPhysics, pos, { 100.0, 0.0, 0.0 },
			{ 0.5, 0.5, 0.5 }, 10.0, 10.0, { 1.0, 1.0, 0.0, 1.0 }, true, 1, "bala"));

		deleteBulletOrLive(true); 
	}
}

void GameSystem::createPlane(Vector3 pos)
{
	plane = new Avion(gScene, gPhysics, pos);

	noPlane = false; 
	
	pos.x -= 5.0; 

	for (int i = 0; i < contadorVidas; i++)
		addUIElement({ float (pos.x + 5.0 * i), 105.0, 0.0 }, false);

	pos.x += 90.0; 

	for (int i = 0; i < cargadorBalas; i++)
	{
		addUIElement({ float(pos.x - 5.0 * i), 105.0, 0.0 }, true);
		contadorBalas++; 
	}
}

void GameSystem::addUIElement(Vector3 pos, bool bala)
{
	if (bala)
		UIBalas.push_front(new Particula(BalasUI(pos)));

	else
		UIVidas.push_back(new Particula(VidasUI(pos)));
}

void GameSystem::createFloor(Vector4 colores, Vector3 pos)
{
	floor.push_back(new Particula(Suelo(colores, pos)));
}

void GameSystem::deleteBulletOrLive(bool bala)
{
	if (bala)
	{
		auto it = UIBalas.begin(); 

		delete (*it);
		it = UIBalas.erase(it);

		contadorBalas--; 
	}

	else
	{
		auto it = UIVidas.begin();

		delete (*it);
		it = UIVidas.erase(it);

		contadorVidas--;
	}
}

//////////////////////////////////////////////////////////////////////

//COLISIONES

void GameSystem::balasVSindestructible(PxActor* bullet)
{
	PxActor* act;
	Rigidbody* p1 = nullptr;

	auto i = balas.begin();

	while (p1 == nullptr && i != balas.end()) 
	{
		act = (*i)->getDinamico();

		if (bullet == act)
		{
			p1 = (*i);
			(*i)->killRigidbody();
		}
			
		else
			++i;
	}
}

void GameSystem::balasVSglobo(PxActor* bala, PxActor* globo)
{
	balasVSindestructible(bala); 
	rigidbodySystem->destruirGlobo(globo); 
}

void GameSystem::balasVSzeppelin(PxActor* bala, PxActor* zeppelin)
{
	balasVSindestructible(bala); 

	PxActor* act;
	Rigidbody* p1 = nullptr;

	auto i = naves.begin();

	while (p1 == nullptr && i != naves.end())
	{
		act = (*i)->getRigidbody()->getEstatico(); 

		if (zeppelin == act)
		{
			p1 = (*i)->getRigidbody();
			particleSystem->generateFireworkSystem((*i)->getPos(), {1.0, 0.5, 0.0, 1.0});
			(*i)->destroy(); 	
		}

		else
			++i;
	}
}

void GameSystem::avionVSglobo(PxActor* globo)
{
	rigidbodySystem->destruirGlobo(globo);

	if (contadorVidas > 0 && !noPlane)
	{
		particleSystem->generateFireworkSystem(plane->getPos(), {1.0, 0.0, 0.0, 1.0});

		deleteBulletOrLive(false);

		posAvion = plane->getPos().x;

		plane->destroy();
		delete plane;

		noPlane = true;
	}		
}