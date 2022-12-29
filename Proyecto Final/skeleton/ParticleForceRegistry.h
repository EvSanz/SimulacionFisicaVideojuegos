#pragma once

#include "Rigidbody.h"
#include "Particula.h"
#include "ForceGenerator.h"
#include "RigidBodyForceGenerator.h"

#include <map>

class ParticleForceRegistry : public std::multimap<ForceGenerator*, Particula*>
{
public:

	void updateForces(double duration);

	void addRegistry(ForceGenerator* fg, Particula* p) { insert({ fg, p }); }

	void deleteParticleRegistry(Particula* p);
};

/////////////////////////////////////////////////////////////////////////

class RigidbodyForceRegistry : public std::multimap<RigidBodyForceGenerator*, Rigidbody*>
{
public:

	void updateRigidbodyForces(double duration); 

	void addForceRegistry(RigidBodyForceGenerator* fg, Rigidbody* p) { insert({ fg, p }); };

	void deleteForceRegistry(Rigidbody* p);
};

