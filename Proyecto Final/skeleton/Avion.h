#pragma once

#include "RenderUtils.hpp"
#include "Rigidbody.h"

class Avion
{
protected:

	Rigidbody* alas; 
	Rigidbody* capsula; 

	bool up = false; 

public:
	
	Avion(PxScene* scene, PxPhysics* physics, Vector3 pos)
	{
		alas = new Rigidbody(scene, physics, pos, { 0.0, 0.0, 0.0 }, { 3.0, 0.1, 7.0 },
			0.0, 50.0, { 1.0, 0.0, 0.0, 1.0 }, false, 1, "avion");
		alas->notAllowedToDie();
		 
		capsula = new Rigidbody(scene, physics, pos, { 0.0, 0.0, 0.0 }, { 2.0, 6.0, 5.0 },
			0.0, 50.0, { 1.0, 1.0, 1.0, 1.0 }, false, 2, "avion");
		capsula->notAllowedToDie(); 
	}

	~Avion()
	{
		delete alas;
		delete capsula;
	}

	void update(double t)
	{
		float movY = 0.0; 

		if (up)
		{
			if (capsula->getPosition().y < 100.0)
				movY = 0.02;
		}

		else
		{
			if (capsula->getPosition().y > 15.0)
				movY = -0.02; 
		}	

		alas->setPosition(alas->getPosition() + Vector3(0.01, movY, 0.0));
		capsula->setPosition(capsula->getPosition() + Vector3(0.01, movY, 0.0));
	}

	void destroy()
	{
		capsula->killRigidbody();
		alas->killRigidbody();
	}

	void changeDirection() { up = !up; }

	Vector3 getPos() { return capsula->getPosition();}
	Vector3 getDir() { return capsula->getPosition().getNormalized(); }
};

//////////////////////////////////////////////////////////////////////////////

class Zeppelin
{
protected:

	Rigidbody* alas;
	Rigidbody* capsula;

public:

	Zeppelin(PxScene* scene, PxPhysics* physics, Vector3 pos) 
	{
		alas = new Rigidbody(scene, physics, pos, { 0.0, 0.0, 0.0 }, { 3.0, 0.1, 7.0 },
			5.0, 50.0, { 0.0, 1.0, 1.0, 1.0 }, false, 1, "zeppelin");
		alas->notAllowedToDie();

		int size = rand() % 6; 

		capsula = new Rigidbody(scene, physics, pos, { 0.0, 0.0, 0.0 }, { 2.0, (float)(4.0 + size), 5.0 },
			5.0, 50.0, { 1.0, 1.0, 1.0, 1.0 }, false, 2, "zeppelin");
		capsula->notAllowedToDie();
	}

	~Zeppelin()
	{
		delete alas; 
		delete capsula; 
	}

	void update(double t)
	{
		alas->setPosition(alas->getPosition() + Vector3(-0.03, 0.0, 0.0));
		capsula->setPosition(capsula->getPosition() + Vector3(-0.03, 0.0, 0.0));
	}

	void destroy()
	{
		capsula->killRigidbody(); 
		alas->killRigidbody(); 
	}

	Rigidbody* getRigidbody() { return capsula; }

	Vector3 getPos() { return capsula->getPosition(); }
	Vector3 getDir() { return capsula->getPosition().getNormalized(); }
};

