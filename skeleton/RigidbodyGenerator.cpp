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
}

std::list<DinamicRigidbody*> UniformBodyGenerator::generateBodies()
{
	std::list<DinamicRigidbody*> listParticles;

	if (model == nullptr)
		return listParticles;

	for (int i = 0; i < nParticulas; i++)
	{
			Vector3 posicion = Vector3(uni_pos.x * dist(gen) + pos.x, uni_pos.y * dist(gen) + pos.y, uni_pos.z * dist(gen) + pos.z);
			Vector3 velocidad = Vector3(uni_vel.x * dist(gen) + vel.x, uni_vel.y * dist(gen) + vel.y, uni_vel.z * dist(gen) + vel.z);

			DinamicRigidbody* p = model->clone();

			p->setVelocity(vel);
			p->setPosition(pos);

			listParticles.push_back(p);
	}

	return listParticles;
}

/////////////////////////////////////////////////////////////////////////

//GAUSSIAN

GaussianBodyGenerator::GaussianBodyGenerator(DinamicRigidbody* m, double genProb, Vector3 auxPos, Vector3 auxVel, int particulas)
{
	model = m;

	activoGauss = false;

	pos = model->getPosition();
	vel = model->getVelocity();

	gen_prob = genProb;

	pos_gauss = auxPos * 2;
	vel_gauss = auxVel * 0.5;

	nParticulas = particulas * 2;
}

std::list<DinamicRigidbody*> GaussianBodyGenerator::generateBodies()
{
	std::list<DinamicRigidbody*> listParticles;

	if (model == nullptr)
		return listParticles;

	for (int i = 0; i < nParticulas; i++)
	{
			Vector3 posicion = Vector3 (pos_gauss.x * dist(gen) + pos.x, pos_gauss.y * dist(gen) + pos.y, pos_gauss.z * dist(gen) + pos.z);
			Vector3 velocidad = Vector3 (vel_gauss.x * dist(gen) + vel.x, vel_gauss.y * dist(gen) + vel.y, vel_gauss.z * dist(gen) + vel.z);

			DinamicRigidbody* p = model->clone();

			p->setPosition(posicion);
			p->setVelocity(velocidad);

			listParticles.push_back(p);
	}

	return listParticles;
}

