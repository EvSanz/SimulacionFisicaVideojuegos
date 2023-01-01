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
	plane->update(t); 

	particleSystem->update(t);
	rigidbodySystem->update(t);

	forceRegistry.updateRigidbodyForces(t);

	for (auto it = naves.begin(); it != naves.end();)
	{
		(*it)->update(t); 

		//if ((*it)->getRigidbody()->getEstatico()->getName() == "CasiMuerto")
		//{
		//	(*it)->loseLife(); 
		//	(*it)->getRigidbody()->getEstatico()->setName("zeppelinMoribundo");
		//}

		if ((*it)->getPos().x + 50.0 < plane->getPos().x || 
			(*it)->getRigidbody()->getEstatico()->getName() == "Destruir")
		{
			delete (*it);
			it = naves.erase(it);
		}

		else
			it++;
	}

	for (auto it = balas.begin(); it != balas.end();)
	{
		(*it)->integrate(t); 

		if (!(*it)->isAlive() || (*it)->getDinamico()->getName() == "Destruir")
		{
			delete (*it);
			it = balas.erase(it);
		}

		else
			it++;
	}

	for (auto it = arboles.begin(); it != arboles.end();)
	{
		if ((*it)->getPosition().x + 50.0 < plane->getPos().x)
		{
			delete (*it);
			it = arboles.erase(it);
		}

		else
			it++;
	}

	for (auto it = floor.begin(); it != floor.end();)
	{
		if ((*it)->getPos().x + 70.0 < plane->getPos().x)
		{
			delete (*it);
			it = floor.erase(it);
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
		Vector3 position = {float ((*it)->getPos().x + 0.01), 105.0, 0.0};
		(*it)->setPosition(position); 
	}

	for (auto it = UIVidas.begin(); it != UIVidas.end(); it++)
	{
		Vector3 position = { float((*it)->getPos().x + 0.01), 105.0, 0.0 };
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
			{ 2.0, (float)tam, 2.0 }, 50.0, 50.0, { 1.0, 0.8, 0.8, 1.0 }, false, 1, "indestructible");
		arboles.push_back(tronco);

		Rigidbody* copa = new Rigidbody(gScene, gPhysics, { posX, (float)(tam + tam / 2), 0.0 }, { 0.0, 0.0, 0.0 },
			{ 10.0, 6.0, 10.0 }, 50.0, 50.0, { 0.0, 1.0, 0.0, 1.0 }, false, 1, "indestructible");
		arboles.push_back(copa);
	}

	else if (obstaculo == 4 || obstaculo == 5)
	{
		int alt = (rand() % 30) + 50;
		rigidbodySystem->createMuelleAnclado(gPhysics, gScene, { posX, (float)alt, 0.0 }); 
	}

	else if (obstaculo == 6 || obstaculo == 7 ||
			 obstaculo == 8)
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

