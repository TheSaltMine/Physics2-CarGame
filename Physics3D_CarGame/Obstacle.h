#pragma once
#include "Primitive.h"
#include "p2DynArray.h"
struct PhysBody3D;

class Obstacle
{
public:
	Obstacle();
	~Obstacle();

	PhysBody3D* body = nullptr;
	Cube shape;

	virtual void Render();
};


class Pendulum : public Obstacle
{
public:
	PhysBody3D* anchor = nullptr;

	Pendulum() {};
	~Pendulum();
};

class Ramp : public Obstacle
{
public:
	Ramp() {};
	~Ramp();
	void Render() override;
	int chunks = 0;
	PhysBody3D** bodies = nullptr;
};
