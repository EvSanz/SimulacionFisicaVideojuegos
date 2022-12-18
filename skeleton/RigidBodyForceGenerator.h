#pragma once

#include "Rigidbody.h"

#include <list>
#include <random>

class RigidBodyForceGenerator
{
public:

	virtual void updateForce(Rigidbody* p, double duration) = 0;

protected:

	double time = -1;
};