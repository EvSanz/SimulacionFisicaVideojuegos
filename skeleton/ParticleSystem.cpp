#include "ParticleSystem.h"

void ParticleSystem::update(double t)
{
	if (!sysFuerzas)
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
				part.erase(it);
			}

			else
				it++;
		}
	}

	else
	{
		force.updateForces(t);

		for (auto it = part.begin(); it != part.end();) 
		{
			(*it)->integrate(t);

			if (!(*it)->isAlive()) 
			{
				force.deleteParticleRegistry(*it);
				delete (*it);
				it = part.erase(it);
			}
			else 
				it++;
		}
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

	int x = rand() % 50; 
	int y = rand() % 50; 
	int z = rand() % 50;
	 
	Particula* i = new Particula(FuegoArtificial(2, colores));

	std::shared_ptr<SphereParticleGenerator> p;
	p.reset(new SphereParticleGenerator({ (float)x, (float)y, (float)z }, i, 40, 200));

	Firework* f = new Firework(FuegoArtificial(4, {0.0, 0.0, 0.0, 1.0}), {p});
	part.push_back(f);
}

void ParticleSystem::generateGravity()
{
	gravityGen = new GravityForceGenerator(Vector3(0.0f, -9.8f, 0.0f));

	Particula* p1 = new Particula(Prueba({ 0.0, 50.0, 20.0 }, 1000, 0.99));
	force.addRegistry(gravityGen, p1);
	part.push_back(p1);

	Particula* p3 = new Particula(Prueba({ 0.0, 50.0, 0.0 }, 1000.0, 0.7));
	force.addRegistry(gravityGen, p3);
	part.push_back(p3);

	Particula* p2 = new Particula(Prueba({ 0.0, 50.0, -20.0 }, 1000, 0.5));
	force.addRegistry(gravityGen, p2);
	part.push_back(p2);
}

void ParticleSystem::generateStorm(int n, int r)
{
	tornadeGen = new TornadeForceGenerator(Vector3(0.0, 0.0, 0.0), r, 5.0);

	for (int i = 0; i < n; i++)
	{
		double x = rand() % (r + 1);
		double y = rand() % (r + 1); 
		double z = rand() % (r + 1);

		Particula* p = new Particula(Prueba({ (float)x, (float)y, (float)z}, r, 0.99));
		force.addRegistry(tornadeGen, p);
		part.push_back(p);
	}
}

void ParticleSystem::generateExplosive(int n, int r)
{
	explodeGen = new ExplosionForceGenerator(Vector3(0.0, 30.0, 0.0), r, 10);

	for (int i = 0; i < n; i++)
	{
		double x = rand() % (r + 1);
		double y = (rand() % (r + 1)) + 30;
		double z = rand() % (r + 1);

		Particula* p = new Particula(Prueba({ (float)x, (float)y, (float)z }, r, 0.99));
		force.addRegistry(explodeGen, p);
		part.push_back(p);
	}
}

void ParticleSystem::generateFogSystem()
{
	Particula* p = new Particula(Gas()); 

	gaussianGen = new GaussianParticleGenerator(p, 0.7, { 5, 5, 5 }, { 0.01, 0.01, 0.01 }, 1000); 

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
	force.clear(); 
}
