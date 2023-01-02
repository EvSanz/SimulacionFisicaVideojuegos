#pragma once

#include "core.hpp"

class ClasesParticulas
{
public:

	Vector4 color;

	Vector3 vel, acc, size, force;

	double damp, mass, time;

	physx::PxTransform pose;

	int disparo; 

	int aleatorio(int a) 
	{ 
		return rand() % (a + 1); 
	}

	bool immortal; 
};

class Prueba : public ClasesParticulas
{
public:

	Prueba(Vector3 pos, double m, double d)
	{
		pose = physx::PxTransform{ pos.x, pos.y, pos.z };
		color = { 1, 1, 1, 1 };
		vel = {0.0, 0.0, 0.0};
		acc = { 0.0, 0.0, 0.0 };
		force = {0.0, 0.0, 0.0};
		size = { 1.0, 1.0, 1.0 };
		time = 50.0;
		mass = m;
		damp = d;
		disparo = 1;
		immortal = false; 
	}
};

class Globo : public ClasesParticulas
{
public:

	Globo(Vector3 pos)
	{
		pose = physx::PxTransform{ pos.x, pos.y, pos.z };
		color = { 1.0, 0.0, 0.0, 1.0 };
		vel = { 0.0, 0.0, 0.0 };
		acc = { 0.0, 0.0, 0.0 };
		force = { 0.0, 0.0, 0.0 };
		size = { 3.0, 1.0, 1.0 };
		time = 50.0;
		mass = 5.0;
		damp = 0.98;
		disparo = 1;
		immortal = true;
	}
}; 

class BalasUI : public ClasesParticulas
{
public:

	BalasUI(Vector3 pos)
	{
		pose = physx::PxTransform{ pos.x, pos.y, pos.z };
		color = { 1.0, 0.5, 0.0, 1.0 };
		vel = { 0.0, 0.0, 0.0 };
		acc = { 0.0, 0.0, 0.0 };
		force = { 0.0, 0.0, 0.0 };
		size = { 1.0, 1.0, 1.0 };
		time = 50.0;
		mass = 5.0;
		damp = 0.98;
		disparo = 1;
		immortal = true;
	}
};

class VidasUI : public ClasesParticulas
{
public:

	VidasUI(Vector3 pos)
	{
		pose = physx::PxTransform{ pos.x, pos.y, pos.z };
		color = { 1.0, 0.0, 0.0, 1.0 };
		vel = { 0.0, 0.0, 0.0 };
		acc = { 0.0, 0.0, 0.0 };
		force = { 0.0, 0.0, 0.0 };
		size = { 1.0, 1.0, 1.0 };
		time = 50.0;
		mass = 5.0;
		damp = 0.98;
		disparo = 1;
		immortal = true;
	}
};

class Suelo : public ClasesParticulas
{
public:

	Suelo(Vector4 colores, Vector3 pos)
	{
		pose = physx::PxTransform{ pos.x, pos.y, pos.z };
		color = colores;
		vel = { 0.0, 0.0, 0.0 };
		acc = { 0.0, 0.0, 0.0 };
		force = { 0.0, 0.0, 0.0 };
		size = { 100.0, 0.1, 100.0 };
		time = 50.0;
		mass = 5.0;
		damp = 0.98;
		disparo = 2;
		immortal = true;
	}
};

//////////////////////////////////////////////////////////////////////////////////////

class PruebaMuelle : public ClasesParticulas
{
public:

	PruebaMuelle(Vector4 colores, Vector3 pos, double m)
	{
		pose = physx::PxTransform{ pos.x, pos.y, pos.z };
		color = colores;
		vel = { 0.0, 0.0, 0.0 };
		acc = { 0.0, 0.0, 0.0 };
		force = { 0.0, 0.0, 0.0 };
		size = {2.0, 2.0, 2.0};
		time = 100.0;
		mass = m;
		damp = 0.9;
		disparo = 1;
		immortal = false;
	}
};

class CajaMuelle : public ClasesParticulas
{
public:

	CajaMuelle(Vector4 colores, Vector3 pos, double m)
	{
		pose = physx::PxTransform{ pos.x, pos.y, pos.z };
		color = colores;
		vel = { 0.0, 0.0, 0.0 };
		acc = { 0.0, 0.0, 0.0 };
		force = { 0.0, 0.0, 0.0 };
		size = { 2.0, 0.5, 2.0 };
		time = 100.0;
		mass = m;
		damp = 0.9;
		disparo = 2;
		immortal = false;
	}
};

class PruebaExp : public ClasesParticulas
{
public:

	PruebaExp(Vector3 pos, Vector4 colores)
	{
		pose = physx::PxTransform{ pos.x, pos.y, pos.z };
		color = colores;
		vel = { 0.0, 0.0, 0.0 };
		acc = { 0.0, 0.0, 0.0 };
		force = { 0.0, 0.0, 0.0 };
		size = { 0.3, 0.3, 0.3 };
		time = 10.0;
		mass = 2.0;
		damp = 0.99;
		disparo = 1;
		immortal = false;
	}
};

class PruebaLineas: public ClasesParticulas
{
public:
	PruebaLineas(Vector3 pos, float r)
	{
		pose = physx::PxTransform{ pos.x, pos.y, pos.z };
		color = { 0, 0, 0, 0.9 };
		vel = { 0.0, 0.0, 0.0 };
		acc = { 0.0, 0.0, 0.0 };
		force = { 0.0, 0.0, 0.0 };
		size = { r, r, r };
		time = 20.0;
		mass = 2.0;
		damp = 0.99;
		disparo = 1;
		immortal = false;
	}
};

class Gun : public ClasesParticulas
{
public:	

	Gun(Vector3 pos, Vector3 dir) 
	{
		pose = physx::PxTransform{ pos.x, pos.y, pos.z };
		color = { 0.3, 0.3, 0.5, 1 };
		vel = dir * 30;
		acc = { 0.0, -1.0, 0.0 };
		force = { 0.0, 0.0, 0.0 };
		size = { 1.0, 0.0, 0.0 };
		time = 1.0;
		mass = 2.0;
		damp = 0.99;
		disparo = 1;
		immortal = false;
	}
}; 

class Canon : public ClasesParticulas
{
public:

	Canon(Vector3 pos, Vector3 dir)
	{
		pose = physx::PxTransform{ pos.x, pos.y, pos.z };
		color = { 0.0f, 0.0f, 1.0f, 1 };
		vel = dir * 100;
		acc = { 0.0, -200.0, 0.0 };
		force = { 0.0, 0.0, 0.0 };
		size = { 3.0, 0.0, 0.0 };
		time = 100.0;
		mass = 200.0;
		damp = 0.99;
		disparo = 1;
		immortal = false;
	}
};

class Fireball : public ClasesParticulas
{
public:

	Fireball(Vector3 pos, Vector3 dir)
	{
		pose = physx::PxTransform{ pos.x, pos.y, pos.z };
		color = { 1.0f, 0.0f, 0.0f, 1 };
		vel = dir * 100;
		acc = { 0.0, 5.0, 0.0 };
		force = { 0.0, 0.0, 0.0 };
		size = { 2.0, 0.0, 0.0 };
		time = 10.0;
		mass = 1.0;
		damp = 0.9;
		disparo = 1;
		immortal = false;
	}
};

class Laser : public ClasesParticulas
{
public:

	Laser(Vector3 pos, Vector3 dir)
	{
		pose = physx::PxTransform{ pos.x, pos.y, pos.z };
		color = { 1.0f, 0.5f, 0.0f, 1 };
		vel = dir * 300;
		acc = { 0.0f, 0.0f, 0.0f };
		force = { 0.0, 0.0, 0.0 };
		size = { 0.75, 0.0, 0.0 };
		time = 5.0;
		mass = 0.01;
		damp = 0.99;
		disparo = 1;
		immortal = false;
	}
};

class Gas : public ClasesParticulas
{
public: 

	Gas(Vector3 pos)
	{
		pose = physx::PxTransform{ pos };
		color = { 1.0f, 1.0f, 1.0f, 1 };
		vel = { 0.0, 0.0, 0.0 };
		acc = { 0.0f, 0.0f, 0.0f };
		force = { 0.0, 0.0, 0.0 };
		size = {0.5, 0.0, 0.0 };
		time = 1.0;
		mass = 1.0;
		damp = 0.8;
		disparo = 1;
		immortal = false;
	}
};

class Agua : public ClasesParticulas
{
public: 
	Agua()
	{
		pose = physx::PxTransform{ 0.0, 3.0, 0.0 };
		color = { 0.0f, 0.0f, 0.95, 1 };
		vel = { 2.0, 5.0, 2.0 };
		acc = { 0.0f, -0.08f, 0.0f };
		force = { 0.0, 0.0, 0.0 };
		size = {2.0, 0.0, 0.0 };
		time = 100.0;
		mass = 0.5;
		damp = 0.95;
		disparo = 1;
		immortal = false;
	}
};

class FuegoArtificial : public ClasesParticulas
{
public:

	FuegoArtificial(int tiempo, Vector3 pos, Vector4 colores) 
	{
		pose = physx::PxTransform{ pos };
		color = colores; 
		vel = {0.0, 0.0, 0.0};
		acc = { 0.0f, 0.0f, 0.0f };
		force = { 0.0, 0.0, 0.0 };
		size = { 0.8, 0.0, 0.0 };
		time = tiempo;
		mass = 1.0;
		damp = 0.85;
		disparo = 1;
		immortal = false;
	}
};
