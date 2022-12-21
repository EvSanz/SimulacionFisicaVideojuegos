#pragma once

#include "Rigidbody.h"

#include <list>
#include <random>

class RigidBodyForceGenerator
{
protected:

	double time = -1;

public:

	virtual void updateForce(DinamicRigidbody* p, double duration) = 0;
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

	void updateForce(DinamicRigidbody* p, double t)
	{
		if (p->getInvMass() < 1e-10)
			return;

		p->addForce(gravity * p->getMass());
	}
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

	void updateForce(DinamicRigidbody* p, double t)
	{
		if ((fabs(p->getInvMass()) < 1e-10) || (p->getPosition() - posViento).magnitudeSquared() > pow(rViento, 2))
			return;

		Vector3 v = velViento - p->getVelocity();
		p->addForce(0.2 * v + 0.001 * v.magnitude() * v);
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

	void updateForce(DinamicRigidbody* p, double t)
	{
		Vector3 d = p->getPosition() - centro;

		velViento = k * Vector3(-(d.z), (20 - d.y), d.x);

		WindForceRigidbodyGenerator::updateForce(p, t);
	}
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

	void updateForce(DinamicRigidbody* p, double t)
	{
		if (t >= tiempo || (fabs(p->getInvMass()) < 1e-10)
			|| (p->getPosition() - expPos).magnitudeSquared() > pow(rExplosion, 2))
			return;

		tiempo += t;

		p->addForce((8000 / pow(rExplosion, 2)) * (p->getPosition() - expPos) * pow(2.7, -((t / 1000.0))));
	}
};

///////////////////////////////////////////////////////////////////////////

class SpringForceRigidbodyGenerator : public RigidBodyForceGenerator
{
protected:

	DinamicRigidbody* auxiliar;
	double k, restLenght;

public:

	SpringForceRigidbodyGenerator(DinamicRigidbody* partAux, double auxK, double auxRestLenght)
	{
		restLenght = auxRestLenght;
		auxiliar = partAux;
		k = auxK;
	}

	void updateForce(DinamicRigidbody* p, double t)
	{
		Vector3 force = auxiliar->getPosition() - p->getPosition();

		const float lenght = force.normalize();
		const float deltaX = lenght - restLenght;

		force *= deltaX * k;

		p->addForce(force);
	}
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

	void updateForce(DinamicRigidbody* p, double t)
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