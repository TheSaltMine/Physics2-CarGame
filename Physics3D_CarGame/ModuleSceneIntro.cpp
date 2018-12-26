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

	ramp_cube.color = Red;
	ramp_cube.size = { 10, 1 , 5 };

	ramp_1 = App->physics->AddRamp(ramp_cube, {10, 49.5, 10}, 50, 5, 1, false);

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	pendulum_1 = CreatePendulum({0,26,0}, {25,5,5}, Blue);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	ramp_1.Clear();

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

	for (int i = 0; i < ramp_1.Count(); i++)
	{
		ramp_1[i]->GetTransform(&ramp_cube.transform);
		ramp_cube.Render();
	}

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

void Pendulum::Render()
{
	body->GetTransform(&shape.transform);
	shape.Render();
}
