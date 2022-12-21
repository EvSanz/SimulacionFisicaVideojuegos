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
	double damping = 0.9; 
	float timeToLive, life;

	bool alive = true;

	RenderItem* renderItem;

	PxScene* gScene;
	PxPhysics* gPhysics;
	PxMaterial* mat;

	PxRigidDynamic* rigidbodyDinamico = nullptr;

public:

	Rigidbody(PxPhysics* physics, PxScene* scene, PxMaterial* material, PxRigidDynamic* rb, Vector3 posicion, Vector3 velocity, Vector3 scale,
		float live, double masa) 
	{
		gPhysics = physics; 
		gScene = scene; 
		mat = material; 
		rigidbodyDinamico = rb;

		pos = posicion; 
		vel = velocity; 
		size = scale; 

		timeToLive = live; 
		life = live;

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

	Vector3 getScale() { return size; }

	double getMaxLive() { return life; }

	PxRigidDynamic* getRigidDynamic() { return rigidbodyDinamico; }
	PxPhysics* getPhysics() { return gPhysics; }
	PxScene* getScene() { return gScene; }
	PxMaterial* getMaterial() { return mat; }

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

		rigidbodyDinamico = gPhysics->createRigidDynamic(PxTransform(pos));

		rigidbodyDinamico->setLinearVelocity(vel);
		rigidbodyDinamico->setGlobalPose(PxTransform(pos)); 

		PxShape* shape = gPhysics->createShape(PxSphereGeometry(size.x), *mat);
		rigidbodyDinamico->attachShape(*shape);

		renderItem = new RenderItem(shape, rigidbodyDinamico, colour);

		gScene->addActor(*rigidbodyDinamico);
	};

	~DinamicRigidbody() {} 

	virtual DinamicRigidbody* clone()
	{
		return new DinamicRigidbody(gPhysics, gScene, mat, rigidbodyDinamico, colour, pos, vel, size, life, mass);
	};

	void integrate(float t) 
	{ 
		if (inverseMass <= 0.0f)
			return;

		timeToLive -= t;

		if (timeToLive < 0)
			alive = false; 

		rigidbodyDinamico->clearForce();
	};

	void addForce(Vector3 f) { rigidbodyDinamico->addForce(f); }

	Vector4 getColor() { return colour; }
	void setColor(Vector4 c) { colour = c; }

	Vector3 getVelocity() { return vel; }
	void setVelocity(Vector3 v) 
	{ 
		vel = v; 
		rigidbodyDinamico->setLinearVelocity(vel);
	}

	float getInvMass() { return inverseMass; }
	float getMass() { return mass; }

	Vector3 getPosition() { return pos; }
	void setPosition(Vector3 p) 
	{ 
		pos = p; 
		rigidbodyDinamico->setGlobalPose(PxTransform(pos));
	}
};

