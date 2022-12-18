#pragma once

#include "Rigidbody.h"
#include "Particula.h"
#include "ForceGenerator.h"
#include "RigidBodyForceGenerator.h"

#include <map>

typedef std::pair<ForceGenerator*, Particula*> FRPair;

class ParticleForceRegistry : public std::multimap<ForceGenerator*, Particula*>
{
public:

	void updateForces(double duration);

	void addRegistry(ForceGenerator* fg, Particula* p) { insert({ fg, p }); }

	void deleteParticleRegistry(Particula* p);
};

/////////////////////////////////////////////////////////////////////////

typedef std::pair<RigidBodyForceGenerator*, Rigidbody*> RBFRPair;

class RigidbodyForceRegistry : public std::multimap<RigidBodyForceGenerator*, Rigidbody*>
{
public:

	void updateRigidbodyForces(double duration); 

	void addForceRegistry(RigidBodyForceGenerator* fg, Rigidbody* p) { insert(RBFRPair(fg, p)); };

	void deleteForceRegistry(Rigidbody* p);
};

