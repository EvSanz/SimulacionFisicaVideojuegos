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

	//Objetos
	std::list<Zeppelin*> naves;

	//Interfaz de usuario
	std::list<Particula*> UIVidas;
	std::list<Particula*> UIBalas;

	RigidbodyForceRegistry forceRegistry;
	ParticleForceRegistry particleForceRegistry; 

	ParticleSystem* particleSystem = nullptr; 
	RigidbodySystem* rigidbodySystem = nullptr;

	PxScene* gScene = nullptr; 
	PxPhysics* gPhysics = nullptr; 

	Avion* plane = nullptr;

	int contadorBalas = 5, cargadorBalas = 5, contadorVidas = 3; 

	double tiempoCooldown = 5.0, tiempo = 0.0; 

	float posX = 100.0, posAvion = 30.0; 

	bool noPlane; 

public:

	GameSystem(PxScene* scene, PxPhysics* physics) 
	{ 
		gScene = scene; 
		gPhysics = physics; 

		particleSystem = new ParticleSystem({ 0, 0, 0 }); 
		rigidbodySystem = new RigidbodySystem({ 0, 0, 0 });

		noPlane = true; 
	}

	~GameSystem();

	virtual void update(double t);

	void addObstacles(int obstaculo, bool zeppelin); 
	void shootBullets(); 

	void createFloor(Vector4 colores, Vector3 pos);

	void createPlane(Vector3 pos);
	Avion* getPlane() { return plane; }

	void addUIElement (Vector3 pos, bool bala);
	void deleteBulletOrLive(bool bala);

	float getPosAvion() { return posAvion; }
	int getVidas() { return contadorVidas; }

	////////////////////////////////////////////////////////////

	void balasVSindestructible(PxActor* bala);
	void balasVSglobo(PxActor* bala, PxActor* globo);
	void balasVSzeppelin(PxActor* bala, PxActor* zeppelin); 
	void avionVSglobo(PxActor* globo); 
};