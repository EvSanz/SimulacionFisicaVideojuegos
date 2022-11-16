#pragma once

#include "Particula.h"
#include "ForceGenerator.h"

#include <map>

typedef std::pair<ForceGenerator*, Particula*> FRPair;

class ParticleForceRegistry : public std::multimap<ForceGenerator*, Particula*>
{
public:

	void updateForces(double duration);

	void addRegistry(ForceGenerator* fg, Particula* p) { insert({ fg, p }); }

	void deleteParticleRegistry(Particula* p);
};

