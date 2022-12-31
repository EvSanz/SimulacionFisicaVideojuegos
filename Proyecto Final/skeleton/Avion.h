#pragma once

#include "RenderUtils.hpp"
#include "Rigidbody.h"

class Avion
{
protected:

	physx::PxTransform posAlas;

	Rigidbody* alas; 
	Rigidbody* capsula; 

	bool up = false; 

public:
	
	Avion(PxScene* scene, PxPhysics* physics, Vector3 pos)
	{
		alas = new Rigidbody(scene, physics, pos, { 0.0, 0.0, 0.0 }, { 3.0, 0.1, 7.0 },
			5.0, 50.0, { 1.0, 0.0, 0.0, 1.0 }, false, 1); 
		alas->notAllowedToDie();
		 
		capsula = new Rigidbody(scene, physics, pos, { 0.0, 0.0, 0.0 }, { 2.0, 6.0, 5.0 },
			5.0, 50.0, { 1.0, 1.0, 1.0, 1.0 }, false, 2);
		capsula->notAllowedToDie(); 
	}

	~Avion()
	{
		alas = nullptr;
		capsula = nullptr; 
	}

	void update(double t)
	{
		float movY = 0.0; 

		if (up)
		{
			if (capsula->getPosition().y < 100.0)
				movY = 0.01;
		}

		else
		{
			if (capsula->getPosition().y > 15.0)
				movY = -0.01; 
		}	

		alas->setPosition(alas->getPosition() + Vector3(0.01, movY, 0.0));
		capsula->setPosition(capsula->getPosition() + Vector3(0.01, movY, 0.0));
	}

	void changeDirection() { up = !up; }

	Vector3 getPos() { return capsula->getPosition();}
	Vector3 getDir() { return capsula->getPosition().getNormalized(); }
};
