#pragma once
#include "Primitive.h"
#include "p2DynArray.h"
struct PhysBody3D;


enum OBSTACLE_TYPE{
	BASIC,
	PENDULUM,
	RAMP,
	CURVE,
	ARCH
};

class Obstacle
{
public:
	Obstacle();
	~Obstacle();

	OBSTACLE_TYPE type = BASIC;
	p2DynArray<PhysBody3D*> bodies;
	Cube shape;

	virtual void Render();
};

class Road :public Obstacle
{
public:
	Road() {};
	~Road();
	Cube border_shape;
	void Render();
};
class Pendulum : public Obstacle
{
public:
	PhysBody3D* anchor = nullptr;

	Pendulum() 
	{
		type = PENDULUM;
	};
	~Pendulum();
};

class Ramp : public Obstacle
{
public:
	Ramp() 
	{
		type = RAMP;
	};
	~Ramp();
};

class Curve : public Obstacle
{
public:
	Curve()
	{
		type = CURVE;
	};
	~Curve();
};

class Arch : public Obstacle
{
public:
	Arch()
	{
		type = ARCH;
	};
	~Arch();
	void Render() override;
	Cube column_shape;
};
