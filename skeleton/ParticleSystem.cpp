#include "ParticleSystem.h"

void ParticleSystem::update(double t)
{
	for (auto g : generadores)
	{
		for (auto p : g->generateParticle()) {
			particulas.push_back(p);
		}
	}

	for (auto it = particulas.begin(); it != particulas.end();) 
	{
		(*it)->integrate(t);

		if (!(*it)->isAlive()) 
		{
			/*Firework* f = dynamic_cast<Firework*>(*it);

			if (f != nullptr) 
			{
				for (auto i : f->explode())
					particulas.push_back(i);
			}*/

			delete (*it);
			it = particulas.erase(it);
		}

		else
			it++;
	}
}

ParticleGenerator* ParticleSystem::getParticleGenerator(string t)
{
	if (t == "fuente")
		generateWaterSystem();
	else if (t == "niebla")
		generateFogSystem(); 
	else if (t == "firework")
		generateFireworkSystem(); 
}

void ParticleSystem::generateFireworkSystem()
{
	Particula* i = new Particula(FuegoArtificial(10));

	std::shared_ptr<SphereParticleGenerator> p;

	p.reset(new SphereParticleGenerator({ 0, 30, 0 }, i, 20, 20));

	particulas.push_back(new Firework(FuegoArtificial(10), {p}));
}

void ParticleSystem::generateFogSystem()
{
	GaussianParticleGenerator* niebla = new GaussianParticleGenerator({ 10, 50, 10 }, { 0, 0, 0 }, 200);
	niebla->setParticle(new Particula(Gas()));
	generadores.push_back(niebla);
}

void ParticleSystem::generateWaterSystem()
{
	UniformParticleGenerator* water = new UniformParticleGenerator({ 0, 0, 0 }, { 0, 10, 0 }, 100);
	water->setParticle(new Particula(Agua()));
	generadores.push_back(water); 
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
