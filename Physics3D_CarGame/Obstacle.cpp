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
	type = BASIC;
}


Obstacle::~Obstacle()
{
	delete shape;
	shape = nullptr;

	for (int i = 0; i < bodies.Count(); i++)
	{
		bodies[i] = nullptr;
	}
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


Pendulum::Pendulum(vec3 position, vec3 size, Color color, bool left)
{
	type = PENDULUM;
	Cube anchor_shape(1, 1, 1);
	anchor_shape.SetPos(position.x, position.y, position.z);

	Cube* b = new Cube(size.x, size.y, size.z);
	b->color = color;
	b->SetPos(position.x + size.x, position.y, position.z);
	if (left)
		b->transform.rotate(180, { 0,0,1 });
	shape = b;

	anchor = App->physics->AddBody(anchor_shape, 0);
	bodies.PushBack(App->physics->AddBody(*b, 90000));
	App->physics->AddConstraintHinge(*anchor, *bodies[0], { 0, 0 ,0 }, { -size.x / 2, 0, 0 }, { 0,0,1 }, { 0,0,1 }, true);

}

Pendulum::~Pendulum()
{
	anchor = nullptr;
}


Arch::Arch(vec3 position, vec3 size, float angle, Color color)
{
	type = ARCH;
	Cube* base = new Cube(size.x, size.y, size.z);
	base->SetPos(position.x, position.y + (size.x / 2), position.z);
	base->SetRotation(angle, { 0,1,0 });
	base->color = color;
	bodies.PushBack(App->physics->AddBody(*base, 0));
	shape = base;

	Cube c(size.y, size.x / 2, size.z);
	c.color = color;
	column_shape = c;
	c.SetPos(position.x + (cos(angle*DEGTORAD)*(size.x / 2)), position.y + (size.x / 4), position.z + (sin(angle*DEGTORAD)*(size.x / 2)));
	bodies.PushBack(App->physics->AddBody(c, 0));
	c.SetPos(position.x - (cos(angle*DEGTORAD)*(size.x / 2)), position.y + (size.x / 4), position.z - (sin(angle*DEGTORAD)*(size.x / 2)));
	bodies.PushBack(App->physics->AddBody(c, 0));
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

Road::Road(vec3 position, vec3 size, float angle, Color color)
{
	type = ROAD;
	Cube* road = new Cube(size.x, size.y, size.z);
	road->color = color;
	road->SetPos(position.x, position.y, position.z);
	road->SetRotation(angle, { 0,1,0 });
	shape = road;
	bodies.PushBack(App->physics->AddBody(*road, 0));

	Cube border(size.x / 6, size.y, size.z);
	border.color = Gray;
	border_shape = border;
	float z_offset = angle > 180 ? -abs((sin(angle*DEGTORAD)*(size.x / 2))) : abs((sin(angle*DEGTORAD)*(size.x / 2)));
	float x_offset = angle >90 && angle <270 ? -abs((cos(angle*DEGTORAD)*(size.x / 2))) : abs((cos(angle*DEGTORAD)*(size.x / 2)));

	border.SetRotation(angle, { 0,1,0 });
	border.SetPos(position.x - x_offset, position.y + (size.y / 2), position.z + z_offset);
	bodies.PushBack(App->physics->AddBody(border, 0));
	border.SetPos(position.x + x_offset, position.y + (size.y / 2), position.z - z_offset);
	bodies.PushBack(App->physics->AddBody(border, 0));
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

Ramp::Ramp(vec3 position, vec3 size, int radius, int chunks, int dir, bool horizontal, bool loop, Color color)
{
	type = RAMP;

	Cube* c = new Cube{ size.x, size.y, size.z };
	c->color = color;
	shape = c;

	vec3 circle_center = { position.x, position.y + radius, position.z };
	int angle = 180;
	int angle_offset = 5;
	float loopincr = 0;

	for (int i = 0; i < chunks; i++)
	{
		if (dir == 0)
		{
			angle += angle_offset;
			loopincr += 0.5F;
		}
		else
		{
			angle -= angle_offset;
			loopincr -= 0.5F;
		}

		vec3 next_position;
		if (!horizontal)
		{
			c->SetRotation(angle, { 1, 0, 0 });
			next_position = rotate(circle_center - position, angle, { 1, 0, 0 });
		}
		else
		{
			c->SetRotation(angle, { 0, 0, 1 });
			next_position = rotate(circle_center - position, angle, { 0, 0, 1 });
		}

		if (loop)
		{
			if (!horizontal)
				c->SetPos(next_position.x + position.x + loopincr, next_position.y + position.y, next_position.z + position.z);
			else
				c->SetPos(next_position.x + position.x, next_position.y + position.y, next_position.z + position.z + loopincr);
		}
		else
			c->SetPos(next_position.x + position.x, next_position.y + position.y, next_position.z + position.z);

		PhysBody3D* pbody = App->physics->AddBody(*c, 0);
		bodies.PushBack(pbody);
	}
}
