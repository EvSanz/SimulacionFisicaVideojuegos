#include "RigidbodyGenerator.h"

/////////////////////////////////////////////////////////////////////////

//UNIFORM 

UniformBodyGenerator::UniformBodyGenerator(DinamicRigidbody* m, double genProb, Vector3 auxPos, Vector3 auxVel, int particulas)
{
	model = m;

	activoUniform = false;

	pos = model->getPosition();
	vel = model->getVelocity();

	gen_prob = genProb;

	uni_pos = auxPos / 3;
	uni_vel = auxVel / 3;

	nParticulas = particulas;

	std::random_device r;
	random_generator = std::mt19937(r());
}

std::list<DinamicRigidbody*> UniformBodyGenerator::generateBodies()
{
	std::list<DinamicRigidbody*> listParticles;

	if (model == nullptr)
		return listParticles;

	auto gen = std::uniform_int_distribution<int>(0, 100);
	auto px = std::uniform_real_distribution<float>(pos.x - uni_pos.x / 2, pos.x + uni_pos.x / 2);
	auto py = std::uniform_real_distribution<float>(pos.y - uni_pos.y / 2, pos.y + uni_pos.y / 2);
	auto pz = std::uniform_real_distribution<float>(pos.z - uni_pos.z / 2, pos.z + uni_pos.z / 2);
	auto vx = std::uniform_real_distribution<float>(vel.x - uni_vel.x / 2, vel.x + uni_vel.x / 2);
	auto vy = std::uniform_real_distribution<float>(vel.y - uni_vel.y / 2, vel.y + uni_vel.y / 2);
	auto vz = std::uniform_real_distribution<float>(vel.z - uni_vel.z / 2, vel.z + uni_vel.z / 2);

	for (int i = 0; i < nParticulas; i++)
	{
		int cr = gen(random_generator);

		if (cr <= gen_prob)
		{
			Vector3 pos = { px(random_generator), py(random_generator), pz(random_generator) };
			Vector3 vel = { vx(random_generator), vy(random_generator), vz(random_generator) };

			DinamicRigidbody* p = model->clone();

			p->setVelocity(vel);
			p->setPosition(pos);

			listParticles.push_back(p);
		}
	}

	return listParticles;
}
