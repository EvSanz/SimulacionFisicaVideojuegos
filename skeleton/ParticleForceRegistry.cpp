#include "ParticleForceRegistry.h"

void ParticleForceRegistry::deleteParticleRegistry(Particula* p)
{
	for (auto i = begin(); i != end();)
	{
		if (i->second == p)
		{
			i = erase(i);
		}

		else
			i++; 
	}
}

void ParticleForceRegistry::updateForces(double duration)
{
	for (auto i = begin(); i != end(); i++)
		i->first->updateForce(i->second, duration);
}
