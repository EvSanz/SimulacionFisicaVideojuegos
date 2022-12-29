#include <ctype.h>
#include <PxPhysicsAPI.h>

#include "ClasesParticulas.h"
#include "ParticleSystem.h"
#include "RigidbodySystem.h"
#include "Particula.h"
#include "Avion.h"

#include <vector>
#include <iostream>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"


using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

ParticleSystem* particulasSystem = NULL;
RigidbodySystem* rigidbodySystem = NULL;

RenderItem* item = nullptr; 

Avion* plane = nullptr; 

std::vector <Particula*> particulas; 

std::vector <Rigidbody*> balas;
std::vector <Rigidbody*> indestructibles;
std::vector <Rigidbody*> destructibles;

int puntuacion; 

float posX; 

double timeMax, tiempo; 

// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.0f, 0.0f, 0.0f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	rigidbodySystem = new RigidbodySystem({ 0, 0, 0 });
	particulasSystem = new ParticleSystem({ 0, 0, 0 });

	PxRigidStatic* floor = gPhysics->createRigidStatic(PxTransform({ 0, -2, 0 }));
	PxShape* shape = CreateShape(PxBoxGeometry(500, 0.1, 500)); 
	floor->attachShape(*shape); 
	item = new RenderItem(shape, floor, { 0.0, 0.8, 0.2, 1 }); 
	gScene->addActor(*floor); 

	plane = new Avion(gScene, gPhysics, Vector3(0.0, 30.0, 0.0)); 

	timeMax = 10.0; 
	tiempo = timeMax; 

	puntuacion = 0; 
	posX = 50.0; 

}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds

void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	particulasSystem->update(t);
	rigidbodySystem->update(t);

	plane->update(t); 

	gScene->simulate(t);
	gScene->fetchResults(true);

	tiempo += t; 

	if (tiempo > timeMax)
	{
		int objeto = rand() % 11; 

		if (objeto == 0 || objeto == 1 || objeto == 2 || objeto == 3)
		{
			int tam = rand() % 30; 

			Rigidbody* tronco = new Rigidbody(gScene, gPhysics, {posX, (float)tam / 2, 0.0}, { 0.0, 0.0, 0.0 },
				{ 2.0, (float)tam, 2.0 }, 50.0, 50.0, {1.0, 0.8, 0.8, 1.0}, false, 1);
			Rigidbody* copa = new Rigidbody(gScene, gPhysics, { posX, (float)(tam + tam/2), 0.0}, {0.0, 0.0, 0.0},
				{ 10.0, 3.0, 10.0 }, 50.0, 50.0, { 0.0, 1.0, 0.0, 1.0 }, false, 1); 

			posX += 10.0; 
		}

		else if (objeto == 4 || objeto == 5)
		{
			Rigidbody* globo = new Rigidbody(gScene, gPhysics, { posX, 50.0, 0.0 }, { 0.0, 0.0, 0.0 },
				{ 2.0, 2.0, 2.0 }, 50.0, 50.0, { 1.0, 0.0, 0.0, 1.0 }, false, 0);
		}

		else if (objeto == 6 || objeto == 7 || objeto == 8 || objeto == 9)
		{
			int tam = rand() % 30;

			Rigidbody* nube = new Rigidbody(gScene, gPhysics, { posX, 50.0, 0.0 }, { 0.0, 0.0, 0.0 },
				{ (float)tam, 2.0, 3.0 }, 50.0, 50.0, { 1.0, 1.0, 1.0, 1.0 }, false, 1);

			posX += tam;
		}

		else
		{
			particulasSystem->generateWaterSystem({posX, 5.0, 0.0});
		}

		tiempo = 0.0; 
		posX += 20.0;
	}


	for (auto it = particulas.begin(); it != particulas.end();)
	{
		(*it)->integrate(t);

		if (!(*it)->isAlive())
		{
			delete (*it);
			it = particulas.erase(it);
		}

		else
			it++;
	}

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
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();

	for (auto particula : particulas)
		delete particula;
	particulas.clear(); 

	for (auto sol : balas)
		delete sol;
	balas.clear();

	for (auto sol : destructibles)
		delete sol;
	destructibles.clear();

	for (auto sol : indestructibles)
		delete sol;
	indestructibles.clear();

	delete particulasSystem; 
	delete rigidbodySystem;
	delete plane; 
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	Vector3 pos = plane->getPos() + Vector3(9.0, 0.0, 0.0);

	switch(toupper(key))
	{
		case 'Z':
			plane->changeDirection();
			break;
		case 'X':
			balas.push_back(new Rigidbody(gScene, gPhysics, pos, {100.0, 0.0, 0.0},
				{ 0.5, 0.5, 0.5 }, 10.0, 10.0, { 0.0, 1.0, 1.0, 1.0 }, true, 1)); 
			break;
		case 'C':
			balas.push_back(new Rigidbody(gScene, gPhysics, pos, { 80.0, 0.0, 0.0 },
				{ 1.0, 1.0, 1.0 }, 15.0, 10.0, { 1.0, 1.0, 0.0, 1.0 }, true, 1));
			break; 
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}

void isOnCollision()
{
	/*for (auto it = balas.begin(); it != balas.end();)
	{
		for (auto aux = indestructibles.begin(); aux != indestructibles.end();)
		{

		}
	}*/
}

int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}