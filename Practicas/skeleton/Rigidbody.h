#pragma once

#include <PxPhysicsAPI.h>

#include "core.hpp"
#include "RenderUtils.hpp"

#include <list>
#include <iostream>

using namespace physx;

class Rigidbody
{
protected:

	double timeToLive, timeLive;

	int form; 

	bool alive; 

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

			PxShape* shape; 
			form = forma; 
			
			switch (forma)
			{
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

			colores = color;
			size = scale;
			
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

		if (timeToLive < 0)
			alive = false; 
	};

	bool isAlive() { return alive; }

	void addForce(Vector3 f) 
	{ 
		rigidbodyDinamico->addForce(f);
		std::cout <<"Fuerza: " << f.x << "\n";
		std::cout << "Vel: " << getLinearVelocity().x << "\n";
	}
	void addTorque(Vector3 f) { rigidbodyDinamico->addTorque(f); }

	Vector3 getPosition() { return rigidbodyDinamico->getGlobalPose().p; }
	void setPosition(Vector3 pos) { rigidbodyDinamico->setGlobalPose(PxTransform(pos)); }

	Vector3 getLinearVelocity() { return rigidbodyDinamico->getLinearVelocity(); }
	void setLinearVelocity(Vector3 vel) { rigidbodyDinamico->setLinearVelocity(vel); }
	
	Vector3 getScale() { return size; }
	Vector4 getColor() { return colores; }

	double getTime() { return timeLive; }

	int getForma() { return form; }

	float getMass() { return rigidbodyDinamico->getMass(); }
	float getInvMass() { return rigidbodyDinamico->getInvMass(); }
};

