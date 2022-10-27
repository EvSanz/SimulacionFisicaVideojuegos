#include "ParticleGenerator.h"

/////////////////////////////////////////////////////////////////////////

ParticleGenerator::~ParticleGenerator()
{
	if (modelo != nullptr)
	{
		delete modelo;
		modelo = nullptr;
	}
}

/////////////////////////////////////////////////////////////////////////

//UNIFORM 

UniformParticleGenerator::UniformParticleGenerator(Vector3 auxPos, Vector3 auxVel, int particulas)
{
	pos = modelo->getPos();
	vel = modelo->getVel();

	uni_pos = auxPos / 2;
	uni_vel = auxVel / 2;

	nParticulas = particulas;

	alive = false;

	std::random_device r;
	random_generator = std::mt19937(r());
}

list<Particula*> UniformParticleGenerator::generateParticle()
{
	list<Particula*> listParticles;

	if (modelo == nullptr) 
		return listParticles;

	auto gen = std::uniform_int_distribution<int>(0, 100);
	auto px = std::uniform_real_distribution<float>(pos.x - uni_pos.x, pos.x + uni_pos.x);
	auto py = std::uniform_real_distribution<float>(pos.y - uni_pos.y, pos.y + uni_pos.y);
	auto pz = std::uniform_real_distribution<float>(pos.z - uni_pos.z, pos.z + uni_pos.z);
	auto vx = std::uniform_real_distribution<float>(vel.x - uni_vel.x, vel.x + uni_vel.x);
	auto vy = std::uniform_real_distribution<float>(vel.y - uni_vel.y, vel.y + uni_vel.y);
	auto vz = std::uniform_real_distribution<float>(vel.z - uni_vel.z, vel.z + uni_vel.z);

	for (int i = 0; i < nParticulas; i++) 
	{
		int cr = gen(random_generator);

		if (cr <= gen_prob)
		{
			Vector3 pos = { px(random_generator), py(random_generator), pz(random_generator) };
			Vector3 vel = { vx(random_generator), vy(random_generator), vz(random_generator) };

			Particula* p = new Particula(modelo->getClass()); 

			p->setVelocity(vel);
			p->setPosition(pos);

			listParticles.push_back(p);
		}
	}

	return listParticles;
}

/////////////////////////////////////////////////////////////////////////

//GAUSSIAN 

GaussianParticleGenerator::GaussianParticleGenerator(Vector3 auxPos, Vector3 auxVel, int particulas)
{
	pos = modelo->getPos(); 
	vel = modelo->getVel(); 

	pos_gauss = auxPos;
	vel_gauss = auxVel;

	nParticulas = particulas;

	alive = false;

	std::random_device r;
	random_generator = std::mt19937(r());
}


list<Particula*> GaussianParticleGenerator::generateParticle()
{
	list<Particula*> listParticles;

	if (modelo == nullptr) 
		return listParticles;

	auto gen = std::uniform_int_distribution<int>(0, 100);
	auto px = std::normal_distribution<>(pos.x, pos_gauss.x);
	auto py = std::normal_distribution<>(pos.y, pos_gauss.y);
	auto pz = std::normal_distribution<>(pos.z, pos_gauss.z);
	auto vx = std::normal_distribution<>(vel.x, vel_gauss.x);
	auto vy = std::normal_distribution<>(vel.y, vel_gauss.y);
	auto vz = std::normal_distribution<>(vel.z, vel_gauss.z);

	for (int i = 0; i < nParticulas; i++) 
	{
		int cr = gen(random_generator);

		if (cr <= gen_prob)
		{
			Vector3 pos = { px(random_generator), py(random_generator), pz(random_generator) };
			Vector3 vel = { vx(random_generator), vy(random_generator), vz(random_generator) };

			Particula* p = new Particula(modelo->getClass());

			p->setPosition(pos);
			p->setVelocity(vel);

			listParticles.push_back(p);
		}
	}

	return listParticles;
}

/////////////////////////////////////////////////////////////////////////

//FIREWORKS

SphereParticleGenerator::SphereParticleGenerator(Vector3 position, Particula* model, float radius, int numParticles)
{
	modelo = model;

	vel = model->getVel();
	pos = position;
	acc = model->getAcceleration();

	radio = radius;
	nParticulas = numParticles;

	random = std::uniform_real_distribution<float>(0, 1);
}

std::list<Particula*> SphereParticleGenerator::generateParticle()
{
	list<Particula*> listParticles;

	if (modelo == nullptr)
		return listParticles;

	std::random_device r{};
	std::mt19937 gen{ r() };

	float theta = 0, phi = 0;

	for (int i = 0; i < nParticulas; i++)
	{
		phi = 2 * 3.14 * random(gen);
		theta = 2 * 3.14 * random(gen);

		float x = cos(theta) * sin(phi);
		float y = sin(theta) * sin(phi);
		float z = cos(phi);

		Vector3 vel(x, y, z);

		Particula* p = new Particula(modelo->getClass()); 

		p->setPosition(pos);
		p->setVelocity(vel.getNormalized() * 20);

		listParticles.push_back(p);
	}

	return listParticles;
}
