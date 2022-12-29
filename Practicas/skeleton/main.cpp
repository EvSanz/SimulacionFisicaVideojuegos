#include <ctype.h>
#include <PxPhysicsAPI.h>

#include "ClasesParticulas.h"
#include "ParticleSystem.h"
#include "RigidbodySystem.h"
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
RigidbodySystem* rigidbodySystem = NULL;

Plano* plano = nullptr;
RenderItem* item = nullptr; 

std::vector <Particula*> particulas; 
std::vector <Rigidbody*> solidos;

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

	//plano = new Plano({ 0, -2, 0 }, { 0.0, 0.8, 0.2, 1 });

	PxRigidStatic* floor = gPhysics->createRigidStatic(PxTransform({ 0, -2, 0 }));
	PxShape* shape = CreateShape(PxBoxGeometry(500, 0.1, 500)); 
	floor->attachShape(*shape); 
	item = new RenderItem(shape, floor, { 0.0, 0.8, 0.2, 1 }); 
	gScene->addActor(*floor); 

	//PxRigidDynamic* ball = gPhysics->createRigidDynamic(PxTransform({ 0, 50, 0 }));
	//PxShape* forma = CreateShape(PxSphereGeometry(5.0));
	//ball->attachShape(*forma);
	//item = new RenderItem(forma, ball, { 1.0, 0.0, 0.2, 1 });
	//gScene->addActor(*ball);
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	particulasSystem->update(t);
	rigidbodySystem->update(t);

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

	for (auto it = solidos.begin(); it != solidos.end();)
	{
		(*it)->integrate(t);

		if (!(*it)->isAlive())
		{
			delete (*it);
			it = solidos.erase(it);
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

	for (auto sol : solidos)
		delete sol;
	solidos.clear();

	delete plano;
	delete particulasSystem; 
	delete rigidbodySystem;
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	Vector3 pos = GetCamera()->getTransform().p;
	Vector3 dir = GetCamera()->getDir(); 

	switch(toupper(key))
	{
		//GRAVEDAD SOLIDOS
		//PISTOLA
		case 'C':
		case 'c':
		{
			//particulas.push_back(new Particula(Gun(pos, dir)));
			rigidbodySystem->createGravity(gPhysics, gScene, gMaterial);
			break;
		}

		//SOLIDOS ESFERA GAUSS
		//ARTILLERIA
		case 'V':
		case 'v':
		{
			//particulas.push_back(new Particula(Canon(pos, dir)));
			rigidbodySystem->changeGaussBodyState();

			if (rigidbodySystem->isGaussBodyActive())
				rigidbodySystem->addGauss(gPhysics, gScene, gMaterial);
			break;
		}

		//SOLIDOS ESFERA UNIFORM
		//BOLA DE FUEGO
		case 'B':
		case 'b':
		{
			//particulas.push_back(new Particula(Fireball(pos, dir)));

			rigidbodySystem->changeUniformBodyState();

			if (rigidbodySystem->isUniformBodyActive())
				rigidbodySystem->addUniform(gPhysics, gScene, gMaterial);
			break;
		}

		//LASER
		//BANDA ELASTICA
		//Explosion SOLIDOS
		case 'N':
		case 'n':
		{
			rigidbodySystem->createWind(gPhysics, gScene, gMaterial, 10, 50); 
			//particulasSystem->generateElasticBand(); 
			//particulas.push_back(new Particula(Laser(pos, dir)));
			break;
		}

		//FUERZAS
		case 'm':
		case 'M':
		{
			//particulasSystem->setForceActive(); 
			rigidbodySystem->setForceActive();
			break; 
		}

		//GRAVEDAD
		//AUMENTAR K EN MUELLES
		case 'P':
		case 'p':
		{
			particulasSystem->moreK(); 
			//particulasSystem->generateGravity();
			break;
		}

		//FUEGO ARTIFICIAL 
		//DISMINUIR K EN MUELLES
		case 'I':
		case 'i':
		{
			particulasSystem->lessK();
			//particulasSystem->generateFireworkSystem();
			break;
		}

		//TORNADO
		//MUELLE ANCLADO
		case 'l':
		case 'L':
		{
			//particulasSystem->generateStorm(500, 50);
			particulasSystem->generateMuelleAnclado(); 
			break;
		}

		//EXPLOSION
		//SLINKY
		case 'j':
		case 'J':
		{
			//particulasSystem->generateExplosive(1000, 20);
			particulasSystem->generateBungee(); 
			break;
		}

		//MUELLE
		case 'R':
		case 'r':
		{
			particulasSystem->generateMuelle(); 
			break;
		}

		//GAS
		//FLOTACION
		case 'G':
		case 'g':
		{
			particulasSystem->generateBounyancy(); 

			/*particulasSystem->setFogActive(); 

			if (particulasSystem->isFogActive())
				particulasSystem->generateFogSystem();*/

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