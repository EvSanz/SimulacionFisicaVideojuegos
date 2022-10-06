#include <ctype.h>
#include <PxPhysicsAPI.h>

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

std::vector <Particula*> particulas; 
Plano* plano; 


// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	plano = new Plano({ 0, -2, 0 }, { 0.3, 0.3, 0.3, 1 }); 
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	for (auto i = particulas.begin(); i != particulas.end();)
	{
		(* i)->integrate(t);

		if (!(* i)->isAlive())
		{
			delete (*i); 
			i = particulas.erase(i);
		}
		else 
		{
			i++;
		}
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

	delete plano;
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);
	Vector3 front = camera.q.getBasisVector2().getNormalized();

	bool create = true; 

	Vector4 color; 
	Vector3 vel, acc; 
	float damp, mass;
	int disp; 

	switch(toupper(key))
	{
		case 'C':
		case 'c':
		{
			//Bala pistola
			color = {0.3f, 0.3f, 0.5f, 1}; 
			vel = { 0.0f, 0.0f, 35.0f }; 
			acc = { 0.0f, -1.0f, 0.0f };
			mass = 2.0f; 
			damp = 0.99f; 
			disp = 2; 
			break;
		}

		case 'V':
		case 'v':
		{
			//Artilleria
			color = { 0.0f, 0.0f, 1.0f, 1 };
			vel = { 0.0f, 30.0f, 40.0f };
			acc = { 0.0f, -20.0f, 0.0f };
			mass = 200.0f;
			damp = 0.99f;
			disp = 1;
			break;
		}

		case 'B':
		case 'b':
		{
			//Bola de fuego
			color = { 1.0f, 0.0f, 0.0f, 1 };
			vel = { 0.0f, 0.0f, 10.0f };
			acc = { 0.0f, 0.6f, 0.0f };
			mass = 1.0f;
			damp = 0.9f;
			disp = 1; 
			break;
		}

		case 'N':
		case 'n':
		{
			//Laser
			color = { 1.0f, 0.5f, 0.0f, 1 };
			vel = { 0.0f, 0.0f, 100.0f };
			acc = { 0.0f, 0.0f, 0.0f };
			mass = 0.1f;
			damp = 0.99f;
			disp = 3;
			break;
		}

		default:
			create = false; 
			break;
	}

	if (create)
	{
		Particula* particula = new Particula(camera.p + Vector3(0, 0, 0), front * -40, acc, damp, mass, disp, color);
		particulas.push_back(particula);
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