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

double timeMax, tiempo, tiempoParado, tiempoMaxParado; 

bool noPlane; 

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

	timeMax = 2.0; 
	tiempo = timeMax; 
	tiempoMaxParado = 3.0; 
	tiempoParado = tiempoMaxParado; 

	posX = 100.0; 
	puntuacion = 0; 

	noPlane = true; 

	gameSystem->createFloor(Vector4(0.0, 1.0, 0.0, 1.0), Vector3(40.0, 0.0, 0.0));
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds

void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	if (!noPlane)
	{
		GetCamera()->update();

		tiempo += t;

		if (tiempo > timeMax)
		{
			int obs = rand() % 10;
			int zeppelin = rand() % 9;

			if (zeppelin < 5)
				gameSystem->addObstacles(obs, true);
			else
				gameSystem->addObstacles(obs, false);

			if (gameSystem->getPlane()->getPos().x + 200 > posX)
			{
				gameSystem->createFloor(Vector4(0.0, 1.0, 0.0, 1.0), Vector3(posX, 0.0, 0.0));
				posX += 100.0;
			}

			tiempo = 0;
		}
	}

	gameSystem->update(t); 
	
	gScene->simulate(t);
	gScene->fetchResults(true);
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
			if(!noPlane)
				gameSystem->getPlane()->changeDirection(); 
			break;
		case 'X':
			if (!noPlane)
				gameSystem->shootBullets(); 
			break; 
		case 'S':
			if (!noPlane)
				gameSystem->controlEstela();
			break;
		case 'A':
			if (noPlane)
			{
				if (gameSystem->getVidas() > 0)
				{
					gameSystem->createPlane({ gameSystem->getPosAvion(), 40.0, 0.0 });
					noPlane = false;
				}

				else
					std::cout << "FIN DEL JUEGO\nPUNTUACION OBTENIDA: " << puntuacion << "\n";
			}
			break; 
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);

	if (!noPlane)
	{
		if (actor1->getName() == "bala")
		{
			if (actor2->getName() == "globo")
			{
				gameSystem->balasVSglobo(actor1, actor2);
				puntuacion += 100;
			}

			else if (actor2->getName() == "zeppelin")
			{
				gameSystem->balasVSzeppelin(actor1, actor2);
				puntuacion += 200;
			}

			else
				gameSystem->balasVSindestructible(actor1);
		}

		else if (actor2->getName() == "bala")
		{
			if (actor1->getName() == "globo")
			{
				gameSystem->balasVSglobo(actor2, actor1);
				puntuacion += 100;
			}

			else if (actor1->getName() == "zeppelin")
			{
				gameSystem->balasVSzeppelin(actor2, actor1);
				puntuacion += 200;
			}

			else
				gameSystem->balasVSindestructible(actor2);
		}

		if (actor1->getName() == "avion")
		{
			if (actor2->getName() == "globo")
			{
				gameSystem->avionVSobstaculo(actor2);
				noPlane = true;
			}

			else if (actor2->getName() == "indestructible")
			{
				gameSystem->avionVSobstaculoEstatico(actor2);
				noPlane = true;
			}

			else if (actor2->getName() == "zeppelin")
			{
				gameSystem->avionVSzeppelin(actor2);
				noPlane = true;
			}
		}

		else if (actor2->getName() == "avion")
		{
			if (actor1->getName() == "globo")
			{
				gameSystem->avionVSobstaculo(actor1);
				noPlane = true;
			}

			else if (actor1->getName() == "indestructible")
			{
				gameSystem->avionVSobstaculoEstatico(actor1);
				noPlane = true;
			}

			else if (actor1->getName() == "zeppelin")
			{
				gameSystem->avionVSzeppelin(actor1);
				noPlane = true;
			}
		}
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