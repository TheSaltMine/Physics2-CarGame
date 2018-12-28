#include "PhysBody3D.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"
#include "ModulePlayer.h"
#include "Obstacle.h"

Obstacle::Obstacle()
{
}


Obstacle::~Obstacle()
{
	body = nullptr;
}

void Obstacle::Render()
{
	body->GetTransform(&shape.transform);
	shape.Render();
}


Ramp::~Ramp()
{
	delete[] bodies;
	bodies = nullptr;
}

void Ramp::Render()
{
	for (int i = 0; i < chunks; i++)
	{
		bodies[i]->GetTransform(&shape.transform);
		shape.Render();
	}
}

Pendulum::~Pendulum()
{
	anchor = nullptr;
}

void Curve::Render()
{
	for (int i = 0; i < bodies.Count(); i++)
	{
		bodies[i]->GetTransform(&shape.transform);
		shape.Render();
	}
}
