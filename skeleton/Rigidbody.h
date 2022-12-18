#pragma once

#include <PxPhysicsAPI.h>

#include "core.hpp"
#include "RenderUtils.hpp"

#include <list>

using namespace physx;

class Rigidbody
{
protected:

	PxScene* scene;
	PxPhysics* gPhysics;
	PxShape* shape;
	PxTransform transform;
	PxMaterial* material;
	PxGeometry* geometry;

	RenderItem* renderItem;
	Vector4 colour;

	double timeToLive;

public:

	Rigidbody(PxPhysics* physics, PxScene* scene, PxGeometry* g,
		PxMaterial* m, PxTransform t, Vector4 c, double time); 

	virtual ~Rigidbody()
	{
		shape->release();
		DeregisterRenderItem(renderItem);
		delete renderItem;
	}

	virtual void integrate(float t) {}

	std::list<Rigidbody*> onDeath() { return std::list<Rigidbody*>(); };

	PxShape* getShape() { return shape; };

	float getTimeRigidbody() { return timeToLive; };

	virtual Vector3 getVelocity() { return { 0.0, 0.0, 0.0 }; }
	virtual void setVelocity(Vector3 v) {}

	virtual Vector3 getPosition() { return transform.p; }
	virtual void setPosition(Vector3 p) = 0; 

	PxGeometry* extraerForma(PxGeometryHolder forma)
	{
		switch (forma.getType())
		{
		case PxGeometryType::eSPHERE:
			return new PxSphereGeometry(forma.sphere());
			break;
		default:
			return new PxBoxGeometry(forma.box());
			break;
		}
	}

	virtual Rigidbody* clone() { return this; }
};

class DinamicRigidbody : public Rigidbody
{
protected:

	PxRigidDynamic* rigidbody;

public:

	DinamicRigidbody(PxPhysics* physics, PxScene* scene, PxTransform trans, 
		PxMaterial* mat, PxGeometry* geo, Vector4 color, double time)
		: Rigidbody(physics, scene, geo, mat, trans, color, time)
	{
		rigidbody = gPhysics->createRigidDynamic(transform);

		shape = gPhysics->createShape(*geometry, *material);
		rigidbody->attachShape(*shape);

		renderItem = new RenderItem(shape, rigidbody, colour);

		scene->addActor(*rigidbody);
	};

	~DinamicRigidbody() { scene->removeActor(*rigidbody);}

	virtual DinamicRigidbody* clone()
	{
		return new DinamicRigidbody(gPhysics, scene, transform, 
			material, extraerForma(shape->getGeometry()), colour, timeToLive);
	};

	void integrate(float t) 
	{ 
		timeToLive -= t;
		rigidbody->clearForce();  
	};

	void addForce(Vector3 f) { rigidbody->addForce(f); };

	Vector3 getVelocity() { return rigidbody->getLinearVelocity(); }
	void setVelocity(Vector3 v) { rigidbody->setLinearVelocity(v); }

	float getInvMass() { return 1.0 / rigidbody->getMass(); }
	float getMass() 
	{ 
		float mass = rigidbody->getMass();

		if (mass <= 0.0) return 0.0;
		else return 1.0 / mass;
	}

	Vector3 getPosition() { return rigidbody->getGlobalPose().p; }
	void setPosition(Vector3 p) 
	{ 
		transform.p = p; 
		rigidbody->setGlobalPose(transform); 
	}
};

