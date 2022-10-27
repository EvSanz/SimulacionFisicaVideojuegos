#include "ParticleSystem.h"

void ParticleSystem::update(double t)
{

	if (gaussActivo)
	{
		for (auto p : gaussianGen->generateParticle())
			part.push_back(p);
	}

	if (fireActivo)
	{
		for (auto p : fireGen->generateParticle())
			part.push_back(p);
	}

	if (uniformActivo)
	{
		for (auto p : uniformGen->generateParticle())
			part.push_back(p);
	}

	for (auto it = part.begin(); it != part.end();)
	{
		(*it)->integrate(t);

		if (!(*it)->isAlive()) 
		{
			Firework* f = dynamic_cast<Firework*>(*it);

			if (f != nullptr) 
			{
				for (auto i : f->explode())
					part.push_back(i);
			}

			delete (*it);
			it = part.erase(it);
		}

		else
			it++;
	}
}

ParticleGenerator* ParticleSystem::getParticleGenerator(string t)
{
	for (auto g : generadores) 
	{
		if (g->getName() == t)
			return g;
	}
}

void ParticleSystem::generateFireworkSystem()
{
	Particula* i = new Particula(FuegoArtificial(1));

	fireGen = new SphereParticleGenerator({ 0.01, 30, 0.01 }, i, 20, 20);

	generadores.push_back(fireGen);
}

void ParticleSystem::generateFogSystem()
{
	Particula* p = new Particula(Gas()); 

	gaussianGen = new GaussianParticleGenerator(p, 0.7, { 5, 5, 5 }, { 0.01, 0.01, 0.01 }, 200); 

	generadores.push_back(gaussianGen);
}

void ParticleSystem::generateWaterSystem()
{
	Particula* p = new Particula(Agua());

	uniformGen = new UniformParticleGenerator(p, 0.7, { 0.01, 0, 0.01 }, { 10, 0, 0.01 }, 400);

	generadores.push_back(uniformGen);
}

ParticleSystem::~ParticleSystem()
{
	for (auto p : part)
	{
		delete p;
		p = nullptr;
	}

	for (auto f : generadores) {
		delete f;
		f = nullptr;
	}

	generadores.clear();
	part.clear();
}