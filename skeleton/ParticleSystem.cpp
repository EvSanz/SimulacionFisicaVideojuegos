#include "ParticleSystem.h"

void ParticleSystem::update(double t)
{
	for (auto g : generadores)
	{
		for (auto p : g->generateParticle()) 
			particulas.push_back(p);
	}

	for (auto it = particulas.begin(); it != particulas.end();) 
	{
		(*it)->integrate(t);

		if (!(*it)->isAlive()) 
		{
			Firework* f = dynamic_cast<Firework*>(*it);

			if (f != nullptr) 
			{
				for (auto i : f->explode())
					particulas.push_back(i);
			}

			delete (*it);
			it = particulas.erase(it);
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
	Particula* i = new Particula(FuegoArtificial(10));

	std::shared_ptr<SphereParticleGenerator> p;

	p.reset(new SphereParticleGenerator({ 0.01, 30, 0.01 }, i, 20, 20));

	particulas.push_back(i);
}

void ParticleSystem::generateFogSystem()
{
	Particula* p = new Particula(Gas()); 
	generadores.push_back(new GaussianParticleGenerator(p, 0.7, { 5, 5, 5 }, { 0.01, 0.01, 0.01 }, 200));
}

void ParticleSystem::generateWaterSystem()
{
	Particula* p = new Particula(Agua());
	generadores.push_back(new UniformParticleGenerator(p, 0.7, { 0.01, 0, 0.01 }, {10, 0, 0.01 }, 100));
}

ParticleSystem::~ParticleSystem()
{
	for (auto p : particulas)
	{
		delete p;
		p = nullptr;
	}

	for (auto f : generadores) {
		delete f;
		f = nullptr;
	}

	generadores.clear();
	particulas.clear();
}
