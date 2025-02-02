#include "ParticleSystem.h"

///////////////////////////////////////////////////////////////

//USADOS EN EL PROYECTO FINAL

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

	part.clear();
	force.clear();
	generadores.clear();
}

void ParticleSystem::update(double t)
{
	force.updateForces(t);

	for (std::list<ParticleGenerator*>::iterator it = generadores.begin(); it != generadores.end();)
	{
		if ((*it) == uniformGen)
		{
			if (estela && uniformGen->getPos().x + 10.0 > position.x)
			{
				windGen = new WindForceGenerator({ -15.0, 10.0, 0.0 }, uniformGen->getPos(), 20);

				for (auto p : (*it)->generateParticle())
				{
					force.addRegistry(windGen, p);
					part.push_back(p);
				}

			}
		}
		
		else
		{
			for (auto p : (*it)->generateParticle())
				part.push_back(p);
		}
		
		
		if ((*it)->getPos().x + 20.0 < position.x || !(*it)->isAlive())
		{		
			delete (*it);
			it = generadores.erase(it);
		}

		else
			it++;
	}

	for (std::list<Particula*>::iterator it = part.begin(); it != part.end();)
	{
		(*it)->integrate(t);

		if (!(*it)->isAlive() || (*it)->getPos().x + 100.0 < position.x)
		{
			Firework* f = dynamic_cast<Firework*>(*it);
			if (f != nullptr)
			{
				for (auto i : f->explode())
					part.push_back(i);

			}

			force.deleteParticleRegistry(*it);

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

void ParticleSystem::generateFireworkSystem(Vector3 pos, Vector4 colores, double masa)
{
	Particula* i = new Particula(FuegoArtificial(0.8, pos, colores, masa));
	part.push_back(i);

	std::shared_ptr<SphereParticleGenerator> p;
	p.reset(new SphereParticleGenerator(pos, i, 10, 30));

	Firework* f = new Firework(FuegoArtificial(0.1, pos, colores, masa), { p });
	part.push_back(f);
}

void ParticleSystem::crearSuelo(Vector4 colores, Vector3 pos)
{
	part.push_back(new Particula(Suelo(colores, pos)));
}

void ParticleSystem::generateEstela(Vector3 pos)
{
	Particula* p = new Particula(Gas(pos));
	uniformGen = new UniformParticleGenerator(p, 0.8, pos, { 0.8, 0, 0.0 }, 15);

	windGen = new WindForceGenerator({ -15.0, 10.0, 0.0 }, pos, 30);
	force.addRegistry(windGen, p); 

	part.push_back(p);
	generadores.push_back(uniformGen);
}

///////////////////////////////////////////////////////////////

//NO USADOS EN EL PROYECTO FINAL

//void ParticleSystem::generateFogSystem(Vector3 pos)
//{
//	Particula* p = new Particula(Gas(pos));
//
//	gaussianGen = new GaussianParticleGenerator(p, 0.5, pos, { 0.01, 0.01, 0.01 }, 5);
//
//	generadores.push_back(gaussianGen);
//}
//
//void ParticleSystem::generateMuelle()
//{
//	Particula* p1 = new Particula(PruebaMuelle({ 1.0, 0.0, 0.0, 1.0 }, { -20.0, 40.0, 0.0 }, 10));
//	Particula* p2 = new Particula(PruebaMuelle({ 1.0, 0.5, 0.0, 1.0 }, { 20.0, 40.0, 0.0 }, 30));
//
//	SpringForceGenerator* muelleGen = new SpringForceGenerator(p2, 10, 20);
//	force.addRegistry(muelleGen, p1);
//	SpringForceGenerator* muelleGen2 = new SpringForceGenerator(p1, 10, 10);
//	force.addRegistry(muelleGen2, p2);
//
//	part.push_back(p1);
//	part.push_back(p2); 
//}
//
//void ParticleSystem::generateBounyancy()
//{
//	Particula* p1 = new Particula(PruebaMuelle({ 1.0, 0.0, 0.0, 1.0 }, { 0.0, 0.0, 0.0 }, 30));
//	bungee = new BungeeForceGenerator(40.0, 0.05, 1000); 
//	force.addRegistry(bungee, p1); 
//	part.push_back(p1); 
//}
//
//void ParticleSystem::generateElasticBand()
//{
//	Particula* p1 = new Particula(PruebaMuelle({ 1.0, 0.0, 0.0, 1.0 }, { -20.0, 40.0, 0.0 }, 10));
//	Particula* p2 = new Particula(PruebaMuelle({ 1.0, 0.5, 0.0, 1.0 }, { 20.0, 40.0, 0.0 }, 30));
//
//	ElasticForce* muelleGen = new ElasticForce(3, 20, p2);
//	force.addRegistry(muelleGen, p1);
//	ElasticForce* muelleGen2 = new ElasticForce(3, 20, p1);
//	force.addRegistry(muelleGen2, p2);
//
//	part.push_back(p1);
//	part.push_back(p2);
//}
//
//void ParticleSystem::generateBungee()
//{
//	Particula* p1 = new Particula(PruebaMuelle({1.0, 0.0, 0.0, 1.0}, { 0.0, 80.0, 0.0 }, 10));
//	Particula* p2 = new Particula(PruebaMuelle({1.0, 0.5, 0.0, 1.0}, { 0.0, 70.0, 0.0 }, 10));
//	Particula* p3 = new Particula(PruebaMuelle({ 1.0, 1.0, 0.0, 1.0 }, { 0.0, 60.0, 0.0 }, 10));
//	Particula* p4 = new Particula(PruebaMuelle({ 0.0, 1.0, 0.0, 1.0 }, { 0.0, 50.0, 0.0 }, 10));
//	Particula* p5 = new Particula(PruebaMuelle({ 0.0, 0.0, 1.0, 1.0 }, { 0.0, 40.0, 0.0 }, 10));
//	Particula* p6 = new Particula(PruebaMuelle({ 0.5, 0.0, 1.0, 1.0 }, { 0.0, 30.0, 0.0 }, 10));
//
//	SpringForceGenerator* muelleGen = new SpringForceGenerator(p1, 20, 15); //1 con 2
//	force.addRegistry(muelleGen, p2);
//	SpringForceGenerator* muelleGen2 = new SpringForceGenerator(p2, 20, 15); //2 con 3
//	force.addRegistry(muelleGen2, p3);
//	SpringForceGenerator* muelleGen3 = new SpringForceGenerator(p3, 20, 15); //3 con 2
//	force.addRegistry(muelleGen3, p2);
//	SpringForceGenerator* muelleGen4 = new SpringForceGenerator(p3, 20, 15); //3 con 4
//	force.addRegistry(muelleGen4, p4);
//	SpringForceGenerator* muelleGen5 = new SpringForceGenerator(p4, 20, 15); //4 con 3
//	force.addRegistry(muelleGen5, p3);
//	SpringForceGenerator* muelleGen6 = new SpringForceGenerator(p4, 20, 15); //4 con 5
//	force.addRegistry(muelleGen6, p5);
//	SpringForceGenerator* muelleGen7 = new SpringForceGenerator(p5, 20, 15); //5 con 4
//	force.addRegistry(muelleGen7, p4);
//	SpringForceGenerator* muelleGen8 = new SpringForceGenerator(p5, 20, 15); //5 con 6
//	force.addRegistry(muelleGen8, p6);
//	SpringForceGenerator* muelleGen9 = new SpringForceGenerator(p6, 20, 15); //6 con 5
//	force.addRegistry(muelleGen9, p5);
//
//	part.push_back(p1);
//	part.push_back(p2);
//	part.push_back(p3);
//	part.push_back(p4);
//	part.push_back(p5);
//	part.push_back(p6);
//}
//
//void ParticleSystem::generateMuelleAnclado(Vector3 pos)
//{
//	Particula* p3 = new Particula(Globo(pos));
//	muelleAnclado = new AnchoredSpringFG(20, 20, { pos.x, 90.0, 0.0 }); 
//
//	force.addRegistry(muelleAnclado, p3);
//	part.push_back(p3);
//}
//
//void ParticleSystem::generateGravity()
//{
//	gravityGen = new GravityForceGenerator(Vector3(0.0f, -9.8f, 0.0f));
//
//	Particula* p1 = new Particula(Prueba({ 0.0, 50.0, 20.0 }, 1000.0, 0.99));
//	force.addRegistry(gravityGen, p1);
//	part.push_back(p1);
//
//	Particula* p3 = new Particula(Prueba({ 0.0, 50.0, 0.0 }, 500.0, 0.99));
//	force.addRegistry(gravityGen, p3);
//	part.push_back(p3);
//
//	Particula* p2 = new Particula(Prueba({ 0.0, 50.0, -20.0 }, 100.0, 0.99));
//	force.addRegistry(gravityGen, p2);
//	part.push_back(p2);
//}
//
//void ParticleSystem::generateStorm(int n, int r)
//{
//	tornadeGen = new TornadeForceGenerator(Vector3(0.0, 0.0, 0.0), r, 5.0);
//
//	Particula* guiaGen = new Particula(PruebaLineas({ (float)r/2, (float)r /2, (float)r /2}, (float)r));
//	part.push_back(guiaGen);
//
//	for (int i = 0; i < n; i++)
//	{
//		double x = rand() % (r + 1);
//		double y = rand() % (r + 1); 
//		double z = rand() % (r + 1);
//
//		Particula* p = new Particula(Prueba({ (float)x, (float)y, (float)z}, 1.0, 0.99));
//		force.addRegistry(tornadeGen, p);
//		part.push_back(p);
//	}
//}
//
//void ParticleSystem::generateExplosive(int n, int r)
//{
//	explodeGen = new ExplosionForceGenerator(Vector3(0.0, 30.0, 0.0), r * 2, 10);
//
//	/*Particula* guiaGen = new Particula(PruebaLineas({ (float)r / 2, (float)(r / 2 + 30), (float)r / 2}, (float)r));
//	part.push_back(guiaGen);*/ 
//
//	for (int i = 0; i < n; i++)
//	{
//		double x = rand() % (r + 1);
//		double y = (rand() % (r + 1)) + 30;
//		double z = rand() % (r + 1);
//
//		Particula* p;
//
//		if (x <= r / 3)
//			p = new Particula(PruebaExp({ (float)x, (float)y, (float)z }, { 1, 1, 0, 1 }));	
//		else if (x > r / 3 && x < (r * 2) / 3)
//			p = new Particula(PruebaExp({ (float)x, (float)y, (float)z }, { 1, 0.5, 0, 1 }));
//		else
//			p = new Particula(PruebaExp({ (float)x, (float)y, (float)z }, { 1, 0, 0, 1 }));
//
//		force.addRegistry(explodeGen, p);
//		part.push_back(p);
//	}
//}



