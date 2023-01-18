#pragma once

#include "RenderUtils.hpp"
#include "Rigidbody.h"

class Avion
{
protected:

	Rigidbody* capsula; 

	bool up = false; 

public:
	
	Avion(PxScene* scene, PxPhysics* physics, Vector3 pos)
	{
		capsula = new Rigidbody(scene, physics, pos, { 0.0, 0.0, 0.0 }, { 2.0, 6.0, 5.0 },
			0.0, 50.0, { 1.0, 1.0, 1.0, 1.0 }, true, 2, "avion");
		capsula->notAllowedToDie(); 
	}

	~Avion()
	{
		//capsula->getDinamico()->release();
		delete capsula;
	}

	void update(double t)
	{
		if (up)
		{
			if (capsula->getPosition().y < 100.0)
				capsula->setLinearVelocity({ 0.0, 0.03, 0.0 });
			else
				capsula->setLinearVelocity({ 0.0, 0.0, 0.0 });
		}
			
		else
			if (capsula->getPosition().y > 10.0)
				capsula->setLinearVelocity({ 0.0, -0.03, 0.0 });
			else
				capsula->setLinearVelocity({ 0.0, 0.0, 0.0 });

		Vector3 pos = capsula->getPosition() + Vector3(0.01, 0.0, 0.0) + capsula->getLinearVelocity();

		capsula->setPosition(pos); 
		capsula->integrate(t); 
	}

	void destroy()
	{
		capsula->killRigidbody();
	}

	void changeDirection() { up = !up; }

	Vector3 getPos() { return capsula->getPosition();}
	Vector3 getDir() { return capsula->getPosition().getNormalized(); }
};

//////////////////////////////////////////////////////////////////////////////

class Zeppelin
{
protected:

	Rigidbody* capsula;

	bool colisionDetectada; 

public:

	Zeppelin(PxScene* scene, PxPhysics* physics, Vector3 pos) 
	{
		int size = rand() % 6; 

		capsula = new Rigidbody(scene, physics, pos, { 100.0, 0.0, 0.0 }, { 2.0, (float)(4.0 + size), 5.0 },
			0.0, 50.0, { 1.0, 1.0, 1.0, 1.0 }, true, 2, "zeppelin");
		capsula->notAllowedToDie();

		colisionDetectada = false; 
	}

	~Zeppelin()
	{
		delete capsula; 
	}

	void update(double t)
	{
		capsula->setLinearVelocity({ 0.0, 0.0, 0.0 });
		Vector3 pos = capsula->getPosition() + Vector3(-0.05, 0.0, 0.0) + capsula->getLinearVelocity();
		capsula->setPosition(pos); 	}

	void destroy()
	{
		colisionDetectada = true; 
		capsula->killRigidbody(); 
	}

	Rigidbody* getRigidbody() { return capsula; }

	Vector3 getPos() { return capsula->getPosition(); }
	Vector3 getDir() { return capsula->getPosition().getNormalized(); }

	bool getColisionDetectada() { return colisionDetectada; }
};

