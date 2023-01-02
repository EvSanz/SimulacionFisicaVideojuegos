#pragma once

#include "RenderUtils.hpp"
#include "Rigidbody.h"
#include "Particula.h"

class Zeppelin
{
protected:

	Rigidbody* alas;
	Rigidbody* capsula;

	std::list<Particula*> UIVidas;

	int vidas = 3; 

public:

	Zeppelin(PxScene* scene, PxPhysics* physics, Vector3 pos)
	{
		for (int i = 0; i < vidas; i++)
			UIVidas.push_back(new Particula(VidasUI({(float)(pos.x + 3.0 * i), (float)(pos.y + 5.0), pos.z})));

		alas = new Rigidbody(scene, physics, pos, { 0.0, 0.0, 0.0 }, { 3.0, 0.1, 7.0 },
			5.0, 50.0, { 0.0, 1.0, 1.0, 1.0 }, false, 1, "zeppelin");
		alas->notAllowedToDie();

		capsula = new Rigidbody(scene, physics, pos, { 0.0, 0.0, 0.0 }, { 2.0, 6.0, 5.0 },
			5.0, 50.0, { 1.0, 1.0, 1.0, 1.0 }, false, 2, "zeppelin");
		capsula->notAllowedToDie();
	}

	~Zeppelin()
	{
		delete alas; 
		delete capsula; 
	}

	Rigidbody* getRigidbody() { return capsula; }

	void update(double t)
	{
		for (auto it = UIVidas.begin(); it != UIVidas.end(); it++)
			(*it)->setPosition((*it)->getPos() + Vector3(-0.03, 0.0, 0.0));

		alas->setPosition(alas->getPosition() + Vector3(-0.03, 0.0, 0.0));
		capsula->setPosition(capsula->getPosition() + Vector3(-0.03, 0.0, 0.0));
	}

	void loseLife() 
	{ 
		auto it = UIVidas.begin();

		delete (*it);
		it = UIVidas.erase(it);

		vidas--; 
	}

	void destroy()
	{
		capsula->killRigidbody();
		alas->killRigidbody(); 
	}

	int getLives() { return vidas; }

	Vector3 getPos() { return capsula->getPosition(); }
	Vector3 getDir() { return capsula->getPosition().getNormalized(); }
};

