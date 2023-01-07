#include "GameSystem.h"

GameSystem::~GameSystem()
{
	for (Particula* b : UIVidas)
		delete b;

	for (Particula* b : UIBalas)
		delete b;

	naves.clear();
	UIVidas.clear();
	UIBalas.clear();

	delete rigidbodySystem; 
	delete particleSystem;
}

void GameSystem::update(double t)
{
	if (!noPlane)
	{
		particleSystem->getPosAvion(plane->getPos()); 
		rigidbodySystem->getPosAvion(plane->getPos());
	}

	particleSystem->update(t);
	rigidbodySystem->update(t);

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

	if (!noPlane)
	{
		tiempo += t;
		if (tiempoCooldown < tiempo && UIBalas.size() < cargadorBalas)
		{
			addUIElement({ float((plane->getPos().x + 85.0) - 5.0 * UIBalas.size()), 105.0, 0.0 }, true);
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
		particleSystem->generateEstela(plane->getPos() - Vector3(8.0, 0.0, 0.0));
	}
}

void GameSystem::addObstacles(int obstaculo, bool zep)
{
	if (obstaculo == 0 || obstaculo == 1 ||
		obstaculo == 2 || obstaculo == 3 ||
		obstaculo == 4 || obstaculo == 5)
		rigidbodySystem->createTree(gPhysics, gScene, posX); 

	else if (obstaculo == 6 || obstaculo == 7 || obstaculo == 8)
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
	if (UIBalas.size() > 0 && !noPlane)
	{
		Vector3 pos = plane->getPos() + Vector3(9.0f, 0.0f, 0.0f);
		rigidbodySystem->createBullet(gPhysics, gScene, pos); 
		deleteBulletOrLive(true); 
	}
}

void GameSystem::createPlane(Vector3 pos)
{
	plane = new Avion(gScene, gPhysics, pos);

	noPlane = false; 

	if (contadorVidas == 3)
	{
		pos.x -= 5.0; 

		for (int i = 0; i < contadorVidas; i++)
			addUIElement({ float(pos.x + 5.0 * i), 105.0, 0.0 }, false);

		pos.x += 90.0;

		for (int i = 0; i < cargadorBalas; i++)
			addUIElement({ float(pos.x - 5.0 * i), 105.0, 0.0 }, true);
	}
}

void GameSystem::addUIElement(Vector3 pos, bool bala)
{
	if (bala)
		UIBalas.push_front(new Particula(BalasUI(pos)));

	else
		UIVidas.push_front(new Particula(VidasUI(pos)));
}

void GameSystem::createFloor(Vector4 colores, Vector3 pos)
{
	particleSystem->crearSuelo(colores, pos); 
}

void GameSystem::deleteBulletOrLive(bool bala)
{
	if (bala)
	{
		if (UIBalas.size() > 0)
		{
			auto it = UIBalas.begin();

			delete (*it);
			it = UIBalas.erase(it);
		}
	}

	else
	{
		if (contadorVidas > 0)
		{
			auto it = UIVidas.begin();

			delete (*it);
			it = UIVidas.erase(it);

			contadorVidas--;
		}
	}
}


//////////////////////////////////////////////////////////////////////

//COLISIONES

void GameSystem::balasVSindestructible(PxActor* bala)
{
	rigidbodySystem->destruirRigido(bala);
}

void GameSystem::balasVSglobo(PxActor* bala, PxActor* globo)
{
	rigidbodySystem->destruirRigido(bala);
	rigidbodySystem->destruirRigido(globo); 
}

void GameSystem::balasVSzeppelin(PxActor* bala, PxActor* zeppelin)
{
	rigidbodySystem->destruirRigido(bala);

	PxActor* act;
	Rigidbody* p1 = nullptr;

	auto i = naves.begin();

	while (p1 == nullptr && i != naves.end())
	{
		act = (*i)->getRigidbody()->getEstatico(); 

		if (zeppelin == act)
		{
			p1 = (*i)->getRigidbody();
			particleSystem->generateFireworkSystem((*i)->getPos(), {1.0, 0.5, 0.0, 1.0}, 5.0);
			(*i)->destroy(); 	
		}

		else
			++i;
	}
}

void GameSystem::avionVSglobo(PxActor* globo)
{
	rigidbodySystem->destruirRigido(globo);

	if (!noPlane)
	{
		particleSystem->generateFireworkSystem(plane->getPos(), {1.0, 0.0, 0.0, 1.0}, 2.0);

		deleteBulletOrLive(false);

		posAvion = plane->getPos().x;

		plane->destroy();
		delete plane;

		noPlane = true;
	}		
}