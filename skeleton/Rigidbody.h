#pragma once

#include <PxPhysicsAPI.h>

#include "core.hpp"
#include "RenderUtils.hpp"

#include <list>

using namespace physx;

class Rigidbody
{
protected:

	Vector3 pos, vel, size;
	Vector4 colour;

	double mass, inverseMass;
	float timeToLive;

	bool alive = true;

	RenderItem* renderItem;

	PxScene* gScene;
	PxPhysics* gPhysics;
	PxMaterial* mat;

	PxRigidDynamic* part = nullptr;

public:

	Rigidbody(PxPhysics* physics, PxScene* scene, PxMaterial* material, PxRigidDynamic* rb, Vector3 posicion, Vector3 velocity, Vector3 scale,
		float live, double masa) 
	{
		gPhysics = physics; 
		gScene = scene; 
		mat = material; 
		part = rb; 

		pos = posicion; 
		vel = velocity; 
		size = scale; 

		timeToLive = live; 

		mass = masa; 
		inverseMass = 1.0 / masa; 
	}

	virtual ~Rigidbody()
	{
		DeregisterRenderItem(renderItem);
		delete renderItem;
	}

	virtual void integrate(float t) {}

	bool isAlive() { return alive; }

	float getTimeRigidbody() { return timeToLive; };

	virtual Vector3 getVelocity() { return { 0.0, 0.0, 0.0 }; }
	virtual void setVelocity(Vector3 v) {}

	virtual Vector3 getPosition() { return pos; }
	virtual void setPosition(Vector3 p) = 0; 

	virtual Rigidbody* clone() { return this; }
};

class DinamicRigidbody : public Rigidbody
{

public:

	DinamicRigidbody(PxPhysics* physics, PxScene* scene, PxMaterial* material, PxRigidDynamic* rigidbody, Vector4 color,
		Vector3 position, Vector3 velocity, Vector3 scale, float live, double masa)
		: Rigidbody(physics, scene, material, rigidbody, position, velocity, scale, live, masa)
	{
		colour = color; 

		part = gPhysics->createRigidDynamic(PxTransform(pos));

		PxShape* shape = gPhysics->createShape(PxSphereGeometry(size.x), *mat);
		part->attachShape(*shape);

		renderItem = new RenderItem(shape, part, colour);

		gScene->addActor(*part);
	};

	~DinamicRigidbody() {} 

	virtual DinamicRigidbody* clone()
	{
		return new DinamicRigidbody(gPhysics, gScene, mat, part, colour, pos, vel, size, timeToLive, mass);
	};

	void integrate(float t) 
	{ 
		timeToLive -= t;

		if (timeToLive < 0)
			alive = false; 

		//part->clearForce();  
	};

	void addForce(Vector3 f) { part->addForce(f); };

	Vector3 getVelocity() { return part->getLinearVelocity(); }
	void setVelocity(Vector3 v) { part->setLinearVelocity(v); }

	float getInvMass() { return 1.0 / part->getMass(); }
	float getMass() 
	{ 
		float mass = part->getMass();

		if (mass <= 0.0) return 0.0;
		else return 1.0 / mass;
	}

	Vector3 getPosition() { return part->getGlobalPose().p; }
	void setPosition(Vector3 p) 
	{ 
		pos = p; 
		part->setGlobalPose(PxTransform(pos)); 
	}
};

