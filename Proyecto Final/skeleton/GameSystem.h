#pragma once

#include <list>

#include "Avion.h"
#include "Rigidbody.h"
#include "RigidbodyGenerator.h"
#include "ParticleForceRegistry.h"
#include "RigidBodyForceGenerator.h"

class GameSystem
{
protected:

	std::list<RigidBodyGenerator*> rigidbodyGenerators;
	std::list<RigidBodyForceGenerator*> forceGenerators;

	//Objetos
	std::list<Rigidbody*> destructibles;
	std::list<Rigidbody*> indestructibles;
	std::list<Rigidbody*> balas;

	//Interfaz de usuario
	std::list<Rigidbody*> UIVidas;
	std::list<Rigidbody*> UIBalas;

	RigidbodyForceRegistry forceRegistry;

	PxScene* gScene = nullptr; 
	PxPhysics* gPhysics = nullptr; 

	Avion* plane = nullptr;

	int contadorBalas, cargadorBalas, contadorVidas; 
	double tiempoCooldown, tiempo; 
	float posX; 

public:

	GameSystem(PxScene* scene, PxPhysics* physics) 
	{ 
		gScene = scene; 
		gPhysics = physics; 

		cargadorBalas = 5;
		contadorBalas = 0;

		contadorVidas = 3;

		tiempoCooldown = 5.0;
		tiempo = 0.0; 

		posX = 100.0f; 
	}

	~GameSystem();

	virtual void update(double t);

	void addGenerator(RigidBodyGenerator* rg) { rigidbodyGenerators.push_back(rg); }

	void addObstacles(int obstaculo); 
	void shootBullets(); 

	void createFloor(Vector3 pos);

	void createPlane(Vector3 pos);
	Avion* getPlane() { return plane; }

	void addUIElement (Vector3 pos, bool bala);
	void deleteBulletOrLive(bool bala);

	float getPosX() { return posX; }
};