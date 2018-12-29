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
	PhysBody3D* body = nullptr;
	Cube shape;

	virtual void Render();
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
	void Render() override;
	int chunks = 0;
	PhysBody3D** bodies = nullptr;
};

class Curve : public Obstacle
{
public:
	Curve()
	{
		type = CURVE;
	};
	~Curve();
	void Render() override;
	p2DynArray<PhysBody3D*> bodies;
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
	p2DynArray<PhysBody3D*> bodies;
};
