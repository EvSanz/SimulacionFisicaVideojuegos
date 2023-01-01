#include "ParticleSystem.h"

void ParticleSystem::update(double t)
{
	if (fuerzasActivadas)
		force.updateForces(t);

		if (gaussianGen != nullptr)
		{
			for (auto p : gaussianGen->generateParticle())
				rigidbodyDinamico.push_back(p);
		}

		for (std::list<Particula*>::iterator it = rigidbodyDinamico.begin(); it != rigidbodyDinamico.end();)
		{
			(*it)->integrate(t);

			if (!(*it)->isAlive())
			{
				Firework* f = dynamic_cast<Firework*>(*it);
				if (f != nullptr)
				{
					for (auto i : f->explode())
						rigidbodyDinamico.push_back(i);

				}

				if(fuerzasActivadas)
					force.deleteParticleRegistry(*it);
				if ((*it) != nullptr) delete (*it);
				rigidbodyDinamico.erase(it);
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

void ParticleSystem::generateMuelle()
{
	Particula* p1 = new Particula(PruebaMuelle({ 1.0, 0.0, 0.0, 1.0 }, { -20.0, 40.0, 0.0 }, 10));
	Particula* p2 = new Particula(PruebaMuelle({ 1.0, 0.5, 0.0, 1.0 }, { 20.0, 40.0, 0.0 }, 30));

	SpringForceGenerator* muelleGen = new SpringForceGenerator(p2, 10, 20);
	force.addRegistry(muelleGen, p1);
	SpringForceGenerator* muelleGen2 = new SpringForceGenerator(p1, 10, 10);
	force.addRegistry(muelleGen2, p2);

	rigidbodyDinamico.push_back(p1);
	rigidbodyDinamico.push_back(p2); 
}

void ParticleSystem::generateBounyancy()
{
	Particula* p1 = new Particula(PruebaMuelle({ 1.0, 0.0, 0.0, 1.0 }, { 0.0, 0.0, 0.0 }, 30));
	bungee = new BungeeForceGenerator(40.0, 0.05, 1000); 
	force.addRegistry(bungee, p1); 
	rigidbodyDinamico.push_back(p1); 
}

void ParticleSystem::generateElasticBand()
{
	Particula* p1 = new Particula(PruebaMuelle({ 1.0, 0.0, 0.0, 1.0 }, { -20.0, 40.0, 0.0 }, 10));
	Particula* p2 = new Particula(PruebaMuelle({ 1.0, 0.5, 0.0, 1.0 }, { 20.0, 40.0, 0.0 }, 30));

	ElasticForce* muelleGen = new ElasticForce(3, 20, p2);
	force.addRegistry(muelleGen, p1);
	ElasticForce* muelleGen2 = new ElasticForce(3, 20, p1);
	force.addRegistry(muelleGen2, p2);

	rigidbodyDinamico.push_back(p1);
	rigidbodyDinamico.push_back(p2);
}

void ParticleSystem::generateBungee()
{
	Particula* p1 = new Particula(PruebaMuelle({1.0, 0.0, 0.0, 1.0}, { 0.0, 80.0, 0.0 }, 10));
	Particula* p2 = new Particula(PruebaMuelle({1.0, 0.5, 0.0, 1.0}, { 0.0, 70.0, 0.0 }, 10));
	Particula* p3 = new Particula(PruebaMuelle({ 1.0, 1.0, 0.0, 1.0 }, { 0.0, 60.0, 0.0 }, 10));
	Particula* p4 = new Particula(PruebaMuelle({ 0.0, 1.0, 0.0, 1.0 }, { 0.0, 50.0, 0.0 }, 10));
	Particula* p5 = new Particula(PruebaMuelle({ 0.0, 0.0, 1.0, 1.0 }, { 0.0, 40.0, 0.0 }, 10));
	Particula* p6 = new Particula(PruebaMuelle({ 0.5, 0.0, 1.0, 1.0 }, { 0.0, 30.0, 0.0 }, 10));

	SpringForceGenerator* muelleGen = new SpringForceGenerator(p1, 20, 15); //1 con 2
	force.addRegistry(muelleGen, p2);
	SpringForceGenerator* muelleGen2 = new SpringForceGenerator(p2, 20, 15); //2 con 3
	force.addRegistry(muelleGen2, p3);
	SpringForceGenerator* muelleGen3 = new SpringForceGenerator(p3, 20, 15); //3 con 2
	force.addRegistry(muelleGen3, p2);
	SpringForceGenerator* muelleGen4 = new SpringForceGenerator(p3, 20, 15); //3 con 4
	force.addRegistry(muelleGen4, p4);
	SpringForceGenerator* muelleGen5 = new SpringForceGenerator(p4, 20, 15); //4 con 3
	force.addRegistry(muelleGen5, p3);
	SpringForceGenerator* muelleGen6 = new SpringForceGenerator(p4, 20, 15); //4 con 5
	force.addRegistry(muelleGen6, p5);
	SpringForceGenerator* muelleGen7 = new SpringForceGenerator(p5, 20, 15); //5 con 4
	force.addRegistry(muelleGen7, p4);
	SpringForceGenerator* muelleGen8 = new SpringForceGenerator(p5, 20, 15); //5 con 6
	force.addRegistry(muelleGen8, p6);
	SpringForceGenerator* muelleGen9 = new SpringForceGenerator(p6, 20, 15); //6 con 5
	force.addRegistry(muelleGen9, p5);

	rigidbodyDinamico.push_back(p1);
	rigidbodyDinamico.push_back(p2);
	rigidbodyDinamico.push_back(p3);
	rigidbodyDinamico.push_back(p4);
	rigidbodyDinamico.push_back(p5);
	rigidbodyDinamico.push_back(p6);
}

void ParticleSystem::generateMuelleAnclado(Vector3 pos)
{
	Particula* p3 = new Particula(Globo(pos));
	muelleAnclado = new AnchoredSpringFG(20, 20, { pos.x, 90.0, 0.0 }); 

	force.addRegistry(muelleAnclado, p3);
	rigidbodyDinamico.push_back(p3);
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
	rigidbodyDinamico.push_back(f);
}

void ParticleSystem::generateGravity()
{
	gravityGen = new GravityForceGenerator(Vector3(0.0f, -9.8f, 0.0f));

	Particula* p1 = new Particula(Prueba({ 0.0, 50.0, 20.0 }, 1000.0, 0.99));
	force.addRegistry(gravityGen, p1);
	rigidbodyDinamico.push_back(p1);

	Particula* p3 = new Particula(Prueba({ 0.0, 50.0, 0.0 }, 500.0, 0.99));
	force.addRegistry(gravityGen, p3);
	rigidbodyDinamico.push_back(p3);

	Particula* p2 = new Particula(Prueba({ 0.0, 50.0, -20.0 }, 100.0, 0.99));
	force.addRegistry(gravityGen, p2);
	rigidbodyDinamico.push_back(p2);
}

void ParticleSystem::generateStorm(int n, int r)
{
	tornadeGen = new TornadeForceGenerator(Vector3(0.0, 0.0, 0.0), r, 5.0);

	Particula* guiaGen = new Particula(PruebaLineas({ (float)r/2, (float)r /2, (float)r /2}, (float)r));
	rigidbodyDinamico.push_back(guiaGen);

	for (int i = 0; i < n; i++)
	{
		double x = rand() % (r + 1);
		double y = rand() % (r + 1); 
		double z = rand() % (r + 1);

		Particula* p = new Particula(Prueba({ (float)x, (float)y, (float)z}, 1.0, 0.99));
		force.addRegistry(tornadeGen, p);
		rigidbodyDinamico.push_back(p);
	}
}

void ParticleSystem::generateExplosive(int n, int r)
{
	explodeGen = new ExplosionForceGenerator(Vector3(0.0, 30.0, 0.0), r * 2, 10);

	/*Particula* guiaGen = new Particula(PruebaLineas({ (float)r / 2, (float)(r / 2 + 30), (float)r / 2}, (float)r));
	part.push_back(guiaGen);*/ 

	for (int i = 0; i < n; i++)
	{
		double x = rand() % (r + 1);
		double y = (rand() % (r + 1)) + 30;
		double z = rand() % (r + 1);

		Particula* p;

		if (x <= r / 3)
			p = new Particula(PruebaExp({ (float)x, (float)y, (float)z }, { 1, 1, 0, 1 }));	
		else if (x > r / 3 && x < (r * 2) / 3)
			p = new Particula(PruebaExp({ (float)x, (float)y, (float)z }, { 1, 0.5, 0, 1 }));
		else
			p = new Particula(PruebaExp({ (float)x, (float)y, (float)z }, { 1, 0, 0, 1 }));

		force.addRegistry(explodeGen, p);
		rigidbodyDinamico.push_back(p);
	}
}

void ParticleSystem::generateFogSystem(Vector3 pos)
{
	Particula* p = new Particula(Gas(pos)); 

	gaussianGen = new GaussianParticleGenerator(p, 0.9, pos, { 0.01, 0.01, 0.01 }, 5); 

	generadores.push_back(gaussianGen);
}

void ParticleSystem::generateWaterSystem(Vector3 pos)
{
	Particula* p = new Particula(Agua());

	uniformGen = new UniformParticleGenerator(p, 0.9, pos, { 5, 0, 0.01 }, 200);

	generadores.push_back(uniformGen);
}

ParticleSystem::~ParticleSystem()
{
	for (auto p : rigidbodyDinamico)
	{
		delete p;
		p = nullptr;
	}

	for (auto f : generadores) {
		delete f;
		f = nullptr;
	}

	generadores.clear();
	rigidbodyDinamico.clear();
	force.clear(); 
}
