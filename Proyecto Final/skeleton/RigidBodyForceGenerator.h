#pragma once
#include <iostream>

#include "Rigidbody.h"

#include <list>
#include <random>

class RigidBodyForceGenerator
{
protected:

	double time = -1e10;

public:

	virtual void updateForceRB(Rigidbody* p, double duration) = 0;
	virtual Vector3 getPos() = 0; 
};

///////////////////////////////////////////////////////////////////////////

class GravityForceRigidbodyGenerator : public RigidBodyForceGenerator
{
protected:

	Vector3 gravity;

public:

	GravityForceRigidbodyGenerator(Vector3& gravedad) { gravity = gravedad; }
	~GravityForceRigidbodyGenerator() {}

	inline void setGravity(Vector3 g) { gravity = g; }

	void updateForceRB(Rigidbody* p, double t)
	{
		if (p->getInvMass() < 1e-10)
			return;

		p->addForce(gravity * p->getMass());
	}

	Vector3 getPos() { return { 0.0, 0.0, 0.0 }; }
};

///////////////////////////////////////////////////////////////////////////

class WindForceRigidbodyGenerator : public RigidBodyForceGenerator
{
protected:

	Vector3 velViento, posViento;
	double rViento;

public:

	WindForceRigidbodyGenerator(const Vector3& vel, const Vector3& pos, double r) 
	{ 
		velViento = vel;
		posViento = pos;
		rViento = r;
	}

	Vector3 getVel() { return velViento; }
	Vector3 getPos() { return posViento; }

	void updateForceRB(Rigidbody* p, double t)
	{
		if ((fabs(p->getInvMass()) < 1e-10) || (p->getPosition() - posViento).magnitudeSquared() > pow(rViento, 2))
			return;

		Vector3 v = p->getLinearVelocity();
		Vector3 diffV = velViento - v; 
		Vector3 force = 0.2 * diffV + 0.001 * diffV.magnitude() * diffV;
		
		p->addForce(force);
	}
};

///////////////////////////////////////////////////////////////////////////

class TornadeForceRigidbodyGenerator : public WindForceRigidbodyGenerator
{
protected:

	Vector3 centro;
	double k;

public:

	TornadeForceRigidbodyGenerator(const Vector3& pos, double r, double aux) :
		WindForceRigidbodyGenerator(Vector3(0, 0, 0), pos, r)
	{
		k = aux;
		centro = pos;
	}

	void updateForceRB(Rigidbody* p, double t)
	{
		Vector3 d = p->getPosition() - centro;

		velViento = k * Vector3(-(d.z), (20 - d.y), d.x);

		WindForceRigidbodyGenerator::updateForceRB(p, t);
	}

	Vector3 getPos() { return centro; }
};

///////////////////////////////////////////////////////////////////////////

class ExplosionForceRigidbodyGenerator : public RigidBodyForceGenerator
{
protected:

	Vector3 expPos;
	double rExplosion, tiempo;

public:

	ExplosionForceRigidbodyGenerator(const Vector3& pos, double r, double duracion)
	{
		expPos = pos;
		rExplosion = r;
		tiempo = duracion;
	}

	void updateForceRB(Rigidbody* p, double t)
	{
		if (t >= tiempo || (fabs(p->getInvMass()) < 1e-10)
			|| (p->getPosition() - expPos).magnitudeSquared() > pow(rExplosion, 2))
			return;

		tiempo += t;

		p->addForce((8000 / pow(rExplosion, 2)) * (p->getPosition() - expPos) * pow(2.7, -((t / 1000.0))));
	}

	Vector3 getPos() { return expPos; }
};

///////////////////////////////////////////////////////////////////////////

class SpringForceRigidbodyGenerator : public RigidBodyForceGenerator
{
protected:

	Rigidbody* auxiliar;
	double k, restLenght;

public:

	SpringForceRigidbodyGenerator(Rigidbody* partAux, double auxK, double auxRestLenght)
	{
		restLenght = auxRestLenght;
		auxiliar = partAux;
		k = auxK;
	}

	void updateForceRB(Rigidbody* p, double t)
	{
		Vector3 force = auxiliar->getPosition() - p->getPosition();

		const float lenght = force.normalize();
		const float deltaX = lenght - restLenght;

		force *= deltaX * k;

		p->addForce(force);
	}

	Vector3 getPos() { return auxiliar->getPosition(); }
};


class AnchoredSpringRigidbodyGenerator : public SpringForceRigidbodyGenerator
{
public:

	AnchoredSpringRigidbodyGenerator(PxScene* gScene, PxPhysics* gPhysics, double k, double resting, const Vector3& anchor_pos)
		: SpringForceRigidbodyGenerator(nullptr, k, resting) 
	{
		auxiliar = new Rigidbody(gScene, gPhysics, anchor_pos, { 0.0, 0.0, 0.0 },
			{ 0.1, 0.1, 0.0 }, 0.0, 10, { 0.0, 0.0, 0.0, 1.0 }, false, 0, " ");
	}

	~AnchoredSpringRigidbodyGenerator() { delete auxiliar; }

	Vector3 getPos() { return auxiliar->getPosition(); }
};

///////////////////////////////////////////////////////////////////////////

class BungeeForceRigidbodyGenerator : public RigidBodyForceGenerator
{
protected:

	float altura, volumen, densidadLiquido, gravedad = 9.8;

	Particula* particulaLiquido;

public:

	BungeeForceRigidbodyGenerator(float h, float V, float d)
	{
		altura = h;
		volumen = V;
		densidadLiquido = d;

		particulaLiquido = new Particula(CajaMuelle({ 0.0, 0.0, 0.0, 1.0 }, { 0.0, h, 0.0 }, 30));
	}

	Vector3 getPos() { return particulaLiquido->getPos(); }

	void updateForceRB(Rigidbody* p, double t)
	{
		float immersed = 0.0;
		float h = p->getPosition().y;
		float h0 = particulaLiquido->getPos().y;

		Vector3 f(0, 0, 0);

		if (h - h0 > altura + 0.5)
			immersed = 0.0;
		else if (h0 - h > altura + 0.5)
			immersed = 1.0;
		else
			immersed = (h0 - h) / (altura + 0.5);

		f.y = densidadLiquido * volumen * immersed * gravedad;

		p->addForce(f);
	}
};