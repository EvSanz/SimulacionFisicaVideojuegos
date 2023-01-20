#include "RigidbodyGenerator.h"

/////////////////////////////////////////////////////////////////////////

//UNIFORM 

UniformBodyGenerator::UniformBodyGenerator(PxScene* gScene, PxPhysics* gPhysics, Rigidbody* m, Vector3 auxPos, Vector3 auxVel, int particulas)
{
	model = m;

	pos = model->getPosition();
	vel = model->getLinearVelocity();

	color = model->getColor();

	tam = model->getScale();
	masa = model->getMass();

	vida = model->getTime();

	forma = model->getForma(); 

	fisicas = gPhysics;
	scene = gScene; 

	uni_pos = auxPos / 3;
	uni_vel = auxVel / 3;

	nParticulas = particulas;
}

std::list<Rigidbody*> UniformBodyGenerator::generateBodies()
{
	std::list<Rigidbody*> listParticles;

	if (model == nullptr)
		return listParticles;

	for (int i = 0; i < nParticulas; i++)
	{
			Vector3 posicion = Vector3(uni_pos.x * dist(gen) + pos.x, uni_pos.y * dist(gen) + pos.y, uni_pos.z * dist(gen) + pos.z);
			Vector3 velocidad = Vector3(uni_vel.x * dist(gen) + vel.x, uni_vel.y * dist(gen) + vel.y, uni_vel.z * dist(gen) + vel.z);

			Rigidbody* p = new Rigidbody(scene, fisicas, inertia, /*pos*/pos, /*vel*/vel, /*size*/tam,
				/*mass*/masa, /*time*/vida, /*color*/color, /*dinamic?*/true, /*shape*/1, " ");

			p->setLinearVelocity(vel);
			p->setPosition(pos);

			listParticles.push_back(p);
	}

	return listParticles;
}

/////////////////////////////////////////////////////////////////////////

//GAUSSIAN

GaussianBodyGenerator::GaussianBodyGenerator(PxScene* gScene, PxPhysics* gPhysics, Rigidbody* m, Vector3 auxPos, Vector3 auxVel, int particulas)
{
	model = m;

	pos = model->getPosition();
	vel = model->getLinearVelocity();

	color = model->getColor();

	tam = model->getScale();
	masa = model->getMass();

	vida = model->getTime();

	forma = model->getForma();

	fisicas = gPhysics;
	scene = gScene;

	pos_gauss = auxPos * 2;
	vel_gauss = auxVel * 0.5;

	nParticulas = particulas * 2;
}

std::list<Rigidbody*> GaussianBodyGenerator::generateBodies()
{
	std::list<Rigidbody*> listParticles;

	if (model == nullptr)
		return listParticles;

	for (int i = 0; i < nParticulas; i++)
	{
			Vector3 posicion = Vector3 (pos_gauss.x + dist(gen) + pos.x, pos_gauss.y + dist(gen) + pos.y, pos_gauss.z + dist(gen) + pos.z);
			Vector3 velocidad = Vector3 (vel_gauss.x + dist(gen) + vel.x, vel_gauss.y + dist(gen) + vel.y, vel_gauss.z + dist(gen) + vel.z);

			Rigidbody* p = new Rigidbody(scene, fisicas, inertia, /*pos*/pos, /*vel*/vel, /*size*/tam,
				/*mass*/masa, /*time*/vida, /*color*/color, /*dinamic?*/true, /*shape*/1, " ");

			p->setLinearVelocity(vel);
			p->setPosition(pos);

			listParticles.push_back(p);
	}

	return listParticles;
}

