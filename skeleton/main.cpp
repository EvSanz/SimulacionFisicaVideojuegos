#include <ctype.h>
#include <PxPhysicsAPI.h>

#include "ClasesParticulas.h"
#include "ParticleSystem.h"
#include "Particula.h"
#include "Plano.h"

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
Plano* plano = nullptr;

std::vector <Particula*> particulas; 

bool activado; 

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

	particulasSystem = new ParticleSystem({ 0,0,0 });
	//plano = new Plano({ 0, -2, 0 }, { 0.3, 0.3, 0.3, 1 });
	activado = false; 
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	particulasSystem->update(1.0);
	gScene->simulate(t);
	gScene->fetchResults(true);

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

	delete plano;
	delete particulasSystem; 
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	Vector3 pos = GetCamera()->getTransform().p;
	Vector3 dir = GetCamera()->getDir(); 

	switch(toupper(key))
	{
		//PISTOLA
		case 'C':
		case 'c':
		{
			particulas.push_back(new Particula(Gun(pos, dir)));
			break;
		}

		//ARTILLERIA
		case 'V':
		case 'v':
		{
			particulas.push_back(new Particula(Canon(pos, dir)));
			break;
		}

		//BOLA DE FUEGO
		case 'B':
		case 'b':
		{
			particulas.push_back(new Particula(Fireball(pos, dir)));
			break;
		}

		//LASER
		case 'N':
		case 'n':
		{
			particulas.push_back(new Particula(Laser(pos, dir)));
			break;
		}

		//GAS
		case 'G':
		case 'g':
		{
			particulasSystem->setFogActive(); 

			if (particulasSystem->isFogActive())
				particulasSystem->generateFogSystem();

			break;
		}

		//AGUA
		case 'K':
		case 'k':
		{
			particulasSystem->setWaterActive();

			if (particulasSystem->isWaterActive())
				particulasSystem->generateWaterSystem();

			break;
		}

		//FUEGO ARTIFICIAL 
		case 'I':
		case 'i':
		{
			particulasSystem->generateFireworkSystem();
			break;
		}

		//FUEGO ARTIFICIAL CON SALTO
		/*case 'M':
		case 'm':
		{
			particulasSystem->setFireActive();

			if (particulasSystem->isFireActive())
				particulasSystem->generateFireworksWithJumps();

			break;
		}*/

		default:
			break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
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