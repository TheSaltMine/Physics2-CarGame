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
	delete shape;
	shape = nullptr;

	bodies.Clear();
}

void Obstacle::Render()
{
	for (int i = 0; i < bodies.Count(); i++)
	{
		bodies[i]->GetTransform(&shape->transform);
		shape->Render();
	}
}


Pendulum::~Pendulum()
{
	anchor = nullptr;
}


void Arch::Render()
{
	//needs improvement, very dirt and risky
	bodies[0]->GetTransform(&shape->transform);
	shape->Render();

	bodies[1]->GetTransform(&column_shape.transform);
	column_shape.Render();
	bodies[2]->GetTransform(&column_shape.transform);
	column_shape.Render();
}

void Road::Render()
{
	bodies[0]->GetTransform(&shape->transform);
	shape->Render();

	bodies[1]->GetTransform(&border_shape.transform);
	border_shape.Render();
	bodies[2]->GetTransform(&border_shape.transform);
	border_shape.Render();
}
