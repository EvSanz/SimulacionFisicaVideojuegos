#pragma once

#include <PxPhysicsAPI.h>

#include "core.hpp"
#include "RenderUtils.hpp"

#include <list>

using namespace physx;

class Rigidbody
{
protected:

	double timeToLive, timeLive;

	bool alive, dinamic, cantDie = false; 

	int form; 

	Vector3 size; 
	Vector4 colores; 

	PxRigidDynamic* rigidbodyDinamico;
	PxRigidStatic* rigidbodyEstatico;

	RenderItem* renderItem;

public:

	Rigidbody(PxScene* scene, PxPhysics* physics, Vector3 pos, Vector3 vel, Vector3 scale,
		double mass, double time, Vector4 color, bool dinamico, int forma)
	{
		if (dinamico) 
		{
			rigidbodyDinamico = physics->createRigidDynamic(PxTransform(pos));

			rigidbodyDinamico->setLinearVelocity(vel);
			rigidbodyDinamico->setAngularVelocity({ 0, 0, 0 });
			rigidbodyDinamico->setGlobalPose(PxTransform(pos));
			rigidbodyDinamico->setMass(mass);

			timeToLive = time;
			timeLive = time;

			colores = color;
			size = scale; 

			alive = true; 
			dinamic = true; 

			PxShape* shape; 
			form = forma; 
			
			switch (forma)
			{
			case 2:
				shape = CreateShape(PxCapsuleGeometry(scale.x, scale.y));
				break;
			case 1:
				shape = CreateShape(PxSphereGeometry(scale.x));
				break; 
			case 0:
				shape = CreateShape(PxBoxGeometry(scale.x, scale.y, scale.z));
				break;
			} 

			rigidbodyDinamico->attachShape(*shape);

			PxRigidBodyExt::updateMassAndInertia(*rigidbodyDinamico, 1);

			scene->addActor(*rigidbodyDinamico);

			renderItem = new RenderItem(shape, rigidbodyDinamico, color);
		}

		else
		{
			rigidbodyEstatico = physics->createRigidStatic(PxTransform(pos));
			rigidbodyEstatico->setGlobalPose(PxTransform(pos));

			colores = color;
			size = scale;

			dinamic = false;
			
			PxShape* shape;
			form = forma;

			switch (forma)
			{
			case 0:
				shape = CreateShape(PxSphereGeometry(scale.x));
				break;
			case 1:
				shape = CreateShape(PxBoxGeometry(scale.x, scale.y, scale.z));
				break;
			case 2:
				shape = CreateShape(PxCapsuleGeometry(scale.x, scale.y));
				break;
			}

			rigidbodyEstatico->attachShape(*shape);
			scene->addActor(*rigidbodyEstatico);

			renderItem = new RenderItem(shape, rigidbodyEstatico, color);
		}
	};

	~Rigidbody()
	{
		//rigidbodyDinamico->release();
		DeregisterRenderItem(renderItem);
	} 

	void integrate(float t) 
	{ 
		if (getInvMass() <= 0.0f)
			return;

		timeToLive -= t;

		if (timeToLive < 0 && !cantDie)
			killRigidbody();
	};

	bool isAlive() { return alive; }
	void killRigidbody() { alive = false; }
	
	bool canDie() { return cantDie; }
	void notAllowedToDie() { cantDie != cantDie; }

	void addForce(Vector3 f) { rigidbodyDinamico->addForce(f); }
	void addTorque(Vector3 f) { rigidbodyDinamico->addTorque(f); }

	Vector3 getPosition() 
	{ 
		if (dinamic)
			return rigidbodyDinamico->getGlobalPose().p; 
		else
			return rigidbodyEstatico->getGlobalPose().p;
	}

	void setPosition(Vector3 pos) 
	{ 
		if (dinamic)
			rigidbodyDinamico->setGlobalPose(PxTransform(pos));
		else
			rigidbodyEstatico->setGlobalPose(PxTransform(pos));
	}

	Vector3 getLinearVelocity() { return rigidbodyDinamico->getLinearVelocity(); }
	void setLinearVelocity(Vector3 vel) { rigidbodyDinamico->setLinearVelocity(vel); }
	
	Vector3 getScale() { return size; }
	Vector4 getColor() { return colores; }

	double getTime() { return timeLive; }

	int getForma() { return form; }

	float getMass() { return rigidbodyDinamico->getMass(); }
	float getInvMass() { return rigidbodyDinamico->getInvMass(); }
};

