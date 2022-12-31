#include "GameSystem.h"

GameSystem::~GameSystem()
{
	for (Rigidbody* b : destructibles)
		delete b;

	for (Rigidbody* b : indestructibles)
		delete b;

	for (Rigidbody* b : balas)
		delete b;

	for (RigidBodyGenerator* g : rigidbodyGenerators)
		delete g;

	balas.clear();
	destructibles.clear();
	indestructibles.clear();

	rigidbodyGenerators.clear();
	forceGenerators.clear();
}

void GameSystem::update(double t)
{
	plane->update(t); 

	forceRegistry.updateRigidbodyForces(t);

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

	for (auto it = indestructibles.begin(); it != indestructibles.end();)
	{
		if ((*it)->getPosition().x + (*it)->getScale().x + 50.0 < plane->getPos().x)
		{
			delete (*it);
			it = indestructibles.erase(it);
		}

		else
			it++;
	}

	for (auto it = destructibles.begin(); it != destructibles.end();)
	{
		if ((*it)->getPosition().x + 50.0 < plane->getPos().x)
		{
			forceRegistry.deleteForceRegistry(*it);

			delete (*it);
			it = destructibles.erase(it);
		}

		else
			it++;
	}

	tiempo += t;
	if (tiempoCooldown < tiempo && contadorBalas < cargadorBalas)
	{
		addUIElement({ float((plane->getPos().x + 85.0) - 5.0 * contadorBalas), 105.0, 0.0 }, true);

		contadorBalas++;
		tiempo = 0.0; 
	}

	for (auto it = UIBalas.begin(); it != UIBalas.end(); it++)
	{
		Vector3 position = {float ((*it)->getPosition().x + 0.01), 105.0, 0.0};
		(*it)->setPosition(position); 
	}

	for (auto it = UIVidas.begin(); it != UIVidas.end(); it++)
	{
		Vector3 position = { float((*it)->getPosition().x + 0.01), 105.0, 0.0 };
		(*it)->setPosition(position);
	}
}

void GameSystem::addObstacles(int obstaculo)
{
	if (obstaculo == 0 || obstaculo == 1 ||
		obstaculo == 2 || obstaculo == 3)
	{
		int tam = (rand() % 30) + 10.0;

		Rigidbody* tronco = new Rigidbody(gScene, gPhysics, { posX, (float)tam / 2, 0.0 }, { 0.0, 0.0, 0.0 },
			{ 2.0, (float)tam, 2.0 }, 50.0, 50.0, { 1.0, 0.8, 0.8, 1.0 }, false, 1);
		indestructibles.push_back(tronco);

		Rigidbody* copa = new Rigidbody(gScene, gPhysics, { posX, (float)(tam + tam / 2), 0.0 }, { 0.0, 0.0, 0.0 },
			{ 10.0, 6.0, 10.0 }, 50.0, 50.0, { 0.0, 1.0, 0.0, 1.0 }, false, 1);
		indestructibles.push_back(copa);
	}

	else if (obstaculo == 4 || obstaculo == 5)
	{
		Rigidbody* globo = new Rigidbody(gScene, gPhysics, { posX, 80.0, 0.0 }, { 0.0, 0.0, 0.0 },
			{ 2.0, 2.0, 2.0 }, 5.0, 50.0, { 1.0, 0.0, 0.0, 1.0 }, false, 0);
		destructibles.push_back(globo);
	}

	else if (obstaculo == 6 || obstaculo == 7 ||
			 obstaculo == 8 || obstaculo == 9)
	{
		int tam = rand() % 30;
		int alt = (rand() % 40) + 60;

		Rigidbody* nube = new Rigidbody(gScene, gPhysics, { posX, (float)alt, 0.0 }, { 0.0, 0.0, 0.0 },
			{ (float)tam, 2.0, 3.0 }, 50.0, 50.0, { 1.0, 1.0, 1.0, 1.0 }, false, 1);
		indestructibles.push_back(nube);
	}

	posX += 60.0;
}

void GameSystem::shootBullets()
{
	if (contadorBalas > 0)
	{
		Vector3 pos = plane->getPos() + Vector3(9.0f, 0.0f, 0.0f);

		balas.push_back(new Rigidbody(gScene, gPhysics, pos, { 100.0, 0.0, 0.0 },
			{ 0.5, 0.5, 0.5 }, 10.0, 10.0, { 1.0, 1.0, 0.0, 1.0 }, true, 1));

		deleteBulletOrLive(true); 
	}
}

void GameSystem::createPlane(Vector3 pos)
{
	plane = new Avion(gScene, gPhysics, pos); 
	
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
		UIBalas.push_front(new Rigidbody(gScene, gPhysics, pos, { 0.0, 0.0, 0.0 },
			{ 1.0, 1.0, 1.0 }, 10.0, 10.0, { 1.0, 0.5, 0.0, 1.0 }, false, 0));

	else
		UIVidas.push_back(new Rigidbody(gScene, gPhysics, pos, { 0.0, 0.0, 0.0 },
			{ 1.0, 1.0, 1.0 }, 10.0, 10.0, { 1.0, 0.0, 0.0, 1.0 }, false, 0));
}

void GameSystem::createFloor(Vector3 pos)
{
	indestructibles.push_back(new Rigidbody(gScene, gPhysics, pos, { 0.0, 0.0, 0.0 },
			{ 300.0, 0.1, 300.0 }, 10.0, 10.0, { 0.0, 0.8, 0.1, 1 }, false, 1));
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