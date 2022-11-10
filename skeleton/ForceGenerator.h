#pragma once

#include <random>
#include <list>

#include "Particula.h"

#include "core.hpp"

using namespace std; 

class ForceGenerator
{
protected:

	string name;

	double t = -1e10;

public: 

	virtual void updateForce(Particula* particle, double duracion) = 0; 

	std::string getName() const { return name; }
	void setName(std::string nombre) { name = nombre; }
};

typedef std::pair<ForceGenerator*, Particula*> FRPair;

class ParticleForceRegistry : public std::multimap<ForceGenerator*, Particula*>
{
public:

	void updateForces(double duration)
	{
		for (auto i = begin(); i != end(); i++)
			i->first->updateForce(i->second, duration);
	}

	void addRegistry(ForceGenerator* fg, Particula* p);

	void deleteParticleRegistry(Particula* p);
};

//////////////////////////////////////////////////////////////////////

class GravityForceGenerator : public ForceGenerator
{
protected: 

	Vector3 gravity; 

public: 

	GravityForceGenerator(const Vector3& g); 

	virtual void updateForce(Particula* p, double t);

	inline void setGravity(Vector3 g) { gravity = g; }
};

//////////////////////////////////////////////////////////////////////

class ParticleDragGenerator : public ForceGenerator
{
protected:

	float _k1, _k2; 

public:

	ParticleDragGenerator(); 
	~ParticleDragGenerator();

	virtual void updateForce(Particula* p, double t); 

	inline void setDrag(float k1, float k2) { _k1 = k1; _k2 = k2; }
	inline float getK1() { return (_k1); }
	inline float getK2() { return (_k2); }
};