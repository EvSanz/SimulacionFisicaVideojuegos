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

	if (fireActivo)
	{
		if (t > timer)
		{
			for (auto p : fireGen->generateParticle())
				part.push_back(p);

			timer += rand() % 600; 
		}
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
	int n = rand() % 3;

	Vector4 colores; 

	if (n == 0)
		colores = { 1.0, 0.0, 0.0, 1.0 }; 
	else if (n == 1)
		colores = { 0.0, 1.0, 0.0, 1.0 };
	else
		colores = { 0.0, 0.0, 1.0, 1.0 };
	 
	Particula* i = new Particula(FuegoArtificial(300, colores));

	std::shared_ptr<SphereParticleGenerator> p;
	p.reset(new SphereParticleGenerator({ 5, 5, 0 }, i, 40, 200));

	Firework* f = new Firework(FuegoArtificial(400, {0.0, 0.0, 0.0, 1.0}), {p});
	part.push_back(f);
}

void ParticleSystem::generateFogSystem()
{
	Particula* p = new Particula(Gas()); 

	gaussianGen = new GaussianParticleGenerator(p, 0.7, { 5, 5, 5 }, { 0.01, 0.01, 0.01 }, 1000); 

	generadores.push_back(gaussianGen);
}

void ParticleSystem::generateFireworksWithJumps()
{
	timer = rand() % 600; 

	Particula* p = new Particula(FuegoArtificial(timer, {0.5, 0.3, 0.6, 1}));

	fireGen = new SphereParticleGenerator({ 5, 5, 0 }, p, 40, 200);

	generadores.push_back(fireGen);
}

void ParticleSystem::generateWaterSystem()
{
	Particula* p = new Particula(Agua());

	uniformGen = new UniformParticleGenerator(p, 0.9, { 5, 0, 5 }, { 5, 0, 0.01 }, 400);

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
