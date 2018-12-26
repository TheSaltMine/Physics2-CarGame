#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"
#include "ModulePlayer.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;



	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	pendulum_1 = CreatePendulum({0,26,0}, {25,5,5}, Blue);
	ramp_1 = CreateRamp({ 10, 49.5, 10 }, { 10, 1, 5 }, 50, 5, 1);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	delete ramp_1;
	ramp_1 = nullptr;
	delete pendulum_1;
	pendulum_1 = nullptr;

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		debug_mode = !debug_mode;

	if (!debug_mode)
	{
		//Camera position behind player
		vec3 back_vector = App->player->vehicle->GetBackVector();

		back_vector = back_vector * 15;
		back_vector.y += CAMERA_Y_OFFSET;

		App->camera->Position = back_vector + App->player->vehicle->GetPosition();

		//Camera look at player
		vec3 vehicle_pos = App->player->vehicle->GetPosition();
		vehicle_pos.y += CAMERA_Y_OFFSET;
		App->camera->LookAt(vehicle_pos);
	}

	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	ramp_1->Render();
	pendulum_1->Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

Pendulum* ModuleSceneIntro::CreatePendulum(vec3 position, vec3 size, Color color)
{
	Pendulum* pendulum = new Pendulum();

	Cube anchor_shape(1, 1, 1);
	anchor_shape.SetPos(position.x, position.y, position.z);

	Cube b(size.x, size.y, size.z);
	b.color = color;
	b.SetPos(position.x + size.x, position.y, position.z);
	pendulum->shape = b;

	pendulum->anchor = App->physics->AddBody(anchor_shape, 0);
	pendulum->body = App->physics->AddBody(b, 10000);
	App->physics->AddConstraintHinge(*pendulum->anchor, *pendulum->body, { 0, 0 ,0 }, { -size.x/2, 0, 0 }, { 0,0,1 }, { 0,0,1 }, true);

	return pendulum;
}
Ramp* ModuleSceneIntro::CreateRamp(vec3 position, vec3 size, int radius, int chunks, int dir, bool loop, Color color)
{
	Cube c = {size.x, size.y, size.z};
	c.color = color;
	Ramp* ramp = new Ramp();
	ramp->shape = c;

	vec3 circle_center = { position.x, position.y + radius, position.z };
	int angle = 180;
	int angle_offset = 4;
	float loopincr = 0;

	for (int i = 0; i < chunks; i++)
	{
		if (dir == 0)
		{
			angle += angle_offset;
			loopincr += 0.2F;
		}
		else
		{
			angle -= angle_offset;
			loopincr -= 0.2F;
		}
		vec3 next_position = rotate(circle_center - position, angle, { 1, 0, 0 });
		if (loop)
			c.SetPos(next_position.x + position.x + loopincr, next_position.y + position.y, next_position.z + position.z);
		else
			c.SetPos(next_position.x + position.x, next_position.y + position.y, next_position.z + position.z);

		c.SetRotation(angle, { 1, 0, 0 });

		PhysBody3D* pbody = App->physics->AddBody(c, 0);
		ramp->bodies.PushBack(pbody);
	}

	return ramp;
}

void Pendulum::Render()
{
	body->GetTransform(&shape.transform);
	shape.Render();
}

void Ramp::Render()
{
	for (int i = 0; i < bodies.Count(); i++)
	{
		bodies[i]->GetTransform(&shape.transform);
		shape.Render();
	}
}
