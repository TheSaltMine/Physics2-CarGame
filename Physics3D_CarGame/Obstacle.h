#pragma once
#include "Primitive.h"
#include "p2DynArray.h"
struct PhysBody3D;


enum OBSTACLE_TYPE{
	BASIC,
	PENDULUM,
	RAMP,
	ARCH,
	ROAD
};

class Obstacle
{
public:
	Obstacle();
	~Obstacle();

	OBSTACLE_TYPE type;
	p2DynArray<PhysBody3D*> bodies;
	Primitive* shape;

	virtual void Render();
};

class Road :public Obstacle
{
public:
	Road(vec3 position, vec3 size, float angle = 0.0f, Color color = White);
	~Road() {};
	Cube border_shape;
	void Render();
};
class Pendulum : public Obstacle
{
public:
	PhysBody3D* anchor = nullptr;

	Pendulum(vec3 position, vec3 size, Color color = Blue, bool left = false);
	~Pendulum();
};

class Ramp : public Obstacle
{
public:
	Ramp(vec3 position, vec3 size, int radius, int chunks, int dir = 0, bool horizontal = false, bool loop = false, Color color = Red);
	~Ramp() {};
};

class Arch : public Obstacle
{
public:
	Arch(vec3 position, vec3 size, float angle = 0.0f, Color color = Green);
	~Arch() {};
	void Render() override;
	Cube column_shape;
};
