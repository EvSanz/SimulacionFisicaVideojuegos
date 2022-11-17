#pragma once

#include <random>
#include <list>
#include <map>

#include "Particula.h"

#include "core.hpp"

using namespace std; 

class ForceGenerator
{
protected:

	string name;

	double time = -1e10;

public: 

	virtual void updateForce(Particula* particle, double duracion) = 0; 

	std::string getName() const { return name; }
	void setName(std::string nombre) { name = nombre; }
};

//////////////////////////////////////////////////////////////////////

class GravityForceGenerator : public ForceGenerator
{
protected: 

	Vector3 gravity; 

public: 

	GravityForceGenerator(Vector3& gravedad);
	~GravityForceGenerator() {}

	void updateForce(Particula* p, double time) override;

	inline void setGravity(Vector3 g) { gravity = g; }
};

//////////////////////////////////////////////////////////////////////

class WindForceGenerator : public ForceGenerator
{
protected: 

	Vector3 velViento, posViento; 
	double rViento; 

public: 

	WindForceGenerator(const Vector3& vel, const Vector3& pos, double r);
	virtual ~WindForceGenerator() {}

	void updateForce(Particula* p, double t);

	Vector3 getVel() { return velViento; }
	Vector3 getPos() { return posViento; }
};

//////////////////////////////////////////////////////////////////////

class TornadeForceGenerator : public ForceGenerator
{
protected:

	WindForceGenerator* rafaga = nullptr; 

	double k; 

public:

	TornadeForceGenerator(const Vector3& vel, double r, double k);

	void updateForce(Particula* p, double t); 
};

//////////////////////////////////////////////////////////////////////

class ExplosionForceGenerator : public ForceGenerator
{
protected:

	Vector3 expPos; 
	double rExplosion, tiempo;

public:

	ExplosionForceGenerator(const Vector3& pos, double r, double t);
	virtual ~ExplosionForceGenerator() {}

	void updateForce(Particula* p, double t);
};

///////////////////////////////////////////////////////////////////

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