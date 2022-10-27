#include "ParticleSystem.h"

void ParticleSystem::update(double t)
{

	if (gaussActivo)
	{
		for (auto p : gaussianGen->generateParticle())
			part.push_back(p);
	}

	if (uniformActivo)
	{
		for (auto p : uniformGen->generateParticle())
			part.push_back(p);
	}

	for (std::list<Particula*>::iterator it = part.begin(); it != part.end();)
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

			if ((*it) != nullptr) delete (*it);
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
	Particula* i = new Particula(Gas());

	std::shared_ptr<SphereParticleGenerator> p;
	p.reset(new SphereParticleGenerator({ 0,30,0 }, i, 20, 20));

	Firework* f = new Firework(FuegoArtificial(5), {p});
	part.push_back(f);
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
