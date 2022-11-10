#include "ParticleSystem.h"

void ParticleSystem::update(double t)
{
	if (gravityActive)
		force->updateForce(t);

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

ForceGenerator* ParticleSystem::getForceGenerator(string t)
{
	for (auto g : generadoresFuerza)
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

	int x = rand() % 50; 
	int y = rand() % 50; 
	int z = rand() % 50;
	 
	Particula* i = new Particula(FuegoArtificial(2000, colores));

	std::shared_ptr<SphereParticleGenerator> p;
	p.reset(new SphereParticleGenerator({ (float)x, (float)y, (float)z }, i, 40, 200));

	Firework* f = new Firework(FuegoArtificial(2000, {0.0, 0.0, 0.0, 1.0}), {p});
	part.push_back(f);
}

void ParticleSystem::generateGravity(Vector3 gravedad)
{
	gravity = gravedad;

	force = new ParticleForceRegistry();

	gravityGen  = new GravityForceGenerator(gravity); 

	generadoresFuerza.push_back(gravityForceGen);
}

void ParticleSystem::generateFogSystem()
{
	Particula* p = new Particula(Gas()); 

	force->addRegistry(gravityGen, p);

	gaussianGen = new GaussianParticleGenerator(p, 0.7, { 5, 5, 5 }, { 0.01, 0.01, 0.01 }, 1000); 

	gaussianGen->addParticleForceRegistry(force);

	generadores.push_back(gaussianGen);
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
