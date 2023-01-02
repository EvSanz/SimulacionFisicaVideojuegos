#pragma once

#include <list>

#include "Avion.h"

#include "Particula.h"
#include "PxPhysicsAPI.h"
#include "ForceGenerator.h"
#include "ParticleSystem.h"
#include "ClasesParticulas.h"
#include "ParticleGenerator.h"

#include "Rigidbody.h"
#include "RigidbodySystem.h"
#include "RigidbodyGenerator.h"
#include "ParticleForceRegistry.h"
#include "RigidBodyForceGenerator.h"


class GameSystem
{
protected:

	std::list<RigidBodyGenerator*> rigidbodyGenerators;
	std::list<RigidBodyForceGenerator*> forceGenerators;

	//Objetos
	std::list<Rigidbody*> arboles;
	std::list<Rigidbody*> balas;

	std::list<Zeppelin*> naves;

	//Interfaz de usuario
	std::list<Particula*> UIVidas;
	std::list<Particula*> UIBalas;
	std::list<Particula*> floor;

	RigidbodyForceRegistry forceRegistry;
	ParticleForceRegistry particleForceRegistry; 

	ParticleSystem* particleSystem = nullptr; 
	RigidbodySystem* rigidbodySystem = nullptr;

	PxScene* gScene = nullptr; 
	PxPhysics* gPhysics = nullptr; 

	Avion* plane = nullptr;

	int contadorBalas, cargadorBalas, contadorVidas; 

	double tiempoCooldown, tiempo; 

	float posX, posAvion = 10.0; 

	bool noPlane; 

public:

	GameSystem(PxScene* scene, PxPhysics* physics) 
	{ 
		gScene = scene; 
		gPhysics = physics; 

		particleSystem = new ParticleSystem({ 0, 0, 0 }); 
		rigidbodySystem = new RigidbodySystem({ 0, 0, 0 });

		cargadorBalas = 5;
		contadorBalas = 0;

		contadorVidas = 3;

		tiempoCooldown = 5.0;
		tiempo = 0.0; 

		posX = 100.0f; 

		noPlane = true; 
	}

	~GameSystem();

	virtual void update(double t);

	void addGenerator(RigidBodyGenerator* rg) { rigidbodyGenerators.push_back(rg); }

	void addObstacles(int obstaculo, bool zeppelin); 
	void shootBullets(); 

	void createFloor(Vector4 colores, Vector3 pos);

	void createPlane(Vector3 pos);
	Avion* getPlane() { return plane; }

	void addUIElement (Vector3 pos, bool bala);
	void deleteBulletOrLive(bool bala);

	float getPosAvion() { return posAvion; }

	////////////////////////////////////////////////////////////

	void balasVSindestructible(PxActor* bala);
	void balasVSglobo(PxActor* bala, PxActor* globo);
	void balasVSzeppelin(PxActor* bala, PxActor* zeppelin); 
	void avionVSglobo(PxActor* globo); 
};