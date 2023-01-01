#include <ctype.h>
#include <PxPhysicsAPI.h>

#include "GameSystem.h"
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

GameSystem* gameSystem = nullptr; 

RenderItem* item = nullptr; 

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

	gameSystem = new GameSystem(gScene, gPhysics); 

	gameSystem->createPlane(Vector3(10.0, 40.0, 0.0)); 
	gameSystem->createFloor(Vector4(0.0, 1.0, 0.0, 1.0), Vector3(0.0, 0.0, 0.0)); 

	timeMax = 2.0; 
	tiempo = timeMax; 

	posX = 100.0; 
	puntuacion = 0; 
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds

void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	GetCamera()->update(); 

	gameSystem->update(t); 
	
	gScene->simulate(t);
	gScene->fetchResults(true);

	tiempo += t; 

	if (tiempo > timeMax)
	{
		int obs = rand() % 10; 

		gameSystem->addObstacles(obs);
		gameSystem->createFloor(Vector4(0.0, 1.0, 0.0, 1.0), Vector3(posX, 0.0, 0.0)); 

		posX += 100.0; 
		tiempo = 0; 
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

	delete gameSystem; 
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch(toupper(key))
	{
		case 'Z':
			gameSystem->getPlane()->changeDirection(); 
			break;
		case 'X':
			gameSystem->shootBullets(); 
			break; 
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);

	if (actor1->getName() == "bala")
	{
		if (actor2->getName() == "globo")
		{
			actor2->setName("Destruir");
			puntuacion += 100;
		}

		else if (actor2->getName() == "zeppelin")
		{
			actor2->setName("Destruir");
			puntuacion += 200;
		}
			
		actor1->setName("Destruir"); 
	}
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