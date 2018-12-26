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

	ramp_1 = App->physics->AddRamp(ramp_cube, {10, 24.5, 10}, 25, 8, 1, false);

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

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

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

