#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"
#include "Obstacle.h"
#include "ModulePlayer.h"

#define PI 3.14159265

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

	Cube c = { 10,10,10 };
	c.SetPos(50, 0, 50);
	checkpoint = App->physics->AddBody(c, 0, this);
	checkpoint->SetAsSensor(true);


	obstacles.PushBack(CreateObstacle({ 0,50,0 }, { 50,1,50 }));
	obstacles.PushBack(CreateCurve({ -40,50,25 }, { 50,1,10 }, 90, 180));
	obstacles.PushBack(CreateObstacle({ -90,50,65 }, { 100,1,50 }));






	obstacles.PushBack(CreatePendulum({ 0,76,25 }, { 25,5,5 }, Blue, true));
	obstacles.PushBack(CreatePendulum({ 0,76,35 }, { 25,5,5 }, Blue));
	obstacles.PushBack(CreateRamp({ 0, 100, -20 }, { 10, 1, 5 }, 50, 5, 1));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	for (int i = 0; i < obstacles.Count(); i++)
	{
		delete obstacles[i];
	}
	obstacles.Clear();

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		debug_mode = !debug_mode;

	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		vec3 checkpoint_pos = current_checkpoint->GetPos();
		App->player->vehicle->SetPos(checkpoint_pos.x, checkpoint_pos.y, checkpoint_pos.z);
	}

	if (!debug_mode)
	{
		//Camera position behind player
		vec3 back_vector = App->player->vehicle->GetBackVector();

		float distance_value = (App->player->vehicle->GetKmh()-CAMERA_MIN_SPEED) / ((CAMERA_MAX_SPEED)- CAMERA_MIN_SPEED); // normalize player speed
		camera_distance = max(CAMERA_MIN_DISTANCE,((1 - distance_value)*CAMERA_MIN_DISTANCE) + (distance_value  * CAMERA_MAX_DISTANCE)); //lerp camera distance with normalized player speed

		back_vector.z *= camera_distance;
		back_vector.x *= camera_distance;
		back_vector.y += CAMERA_Y_OFFSET;

		App->camera->Position = back_vector + App->player->vehicle->GetPos();
		App->camera->Reference = back_vector + App->player->vehicle->GetPos();

		//Camera look at player
		vec3 vehicle_pos = App->player->vehicle->GetPos();
		vehicle_pos.y += CAMERA_Y_OFFSET;
		App->camera->LookAt(vehicle_pos);
	}

	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	for (int i = 0; i < obstacles.Count(); i++)
	{
		obstacles[i]->Render();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body2 == checkpoint)
	{
		current_checkpoint = checkpoint;
	}
}

Pendulum* ModuleSceneIntro::CreatePendulum(vec3 position, vec3 size, Color color, bool left)
{
	Pendulum* pendulum = new Pendulum();

	Cube anchor_shape(1, 1, 1);
	anchor_shape.SetPos(position.x, position.y, position.z);

	Cube b(size.x, size.y, size.z);
	b.color = color;
	b.SetPos(position.x + size.x, position.y, position.z);	
	if (left)
		b.transform.rotate(180, { 0,0,1 });
	pendulum->shape = b;

	pendulum->anchor = App->physics->AddBody(anchor_shape, 0);
	pendulum->body = App->physics->AddBody(b, 90000);
	App->physics->AddConstraintHinge(*pendulum->anchor, *pendulum->body, { 0, 0 ,0 }, { -size.x/2, 0, 0 }, { 0,0,1 }, { 0,0,1 }, true);

	return pendulum;
}

Ramp* ModuleSceneIntro::CreateRamp(vec3 position, vec3 size, int radius, int chunks, int dir, bool loop, Color color)
{
	Cube c = {size.x, size.y, size.z};
	c.color = color;
	Ramp* ramp = new Ramp();
	ramp->shape = c;
	ramp->bodies = new PhysBody3D*[chunks];
	ramp->chunks = chunks;

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
		ramp->bodies[i] = pbody;
	}

	return ramp;
}

Obstacle* ModuleSceneIntro::CreateObstacle(vec3 position, vec3 size, Color color)
{
	Obstacle* obstacle = new Obstacle();

	Cube b(size.x, size.y, size.z);
	b.color = color;
	b.SetPos(position.x, position.y, position.z);
	obstacle->shape = b;

	obstacle->body = App->physics->AddBody(b, 0);

	return obstacle;
}

Curve* ModuleSceneIntro::CreateCurve(vec3 position, vec3 size, float initial_angle, float final_angle, Color color)
{
	Curve* curve = new Curve();
	Cube c(size.x, size.y, size.z);
	c.color = color;
	curve->shape = c;
	float increment = 5.f;
	float offset = 15.f;

	while (initial_angle <= final_angle)
	{
		float z_offset = initial_angle > 180 ? -abs((offset + (size.x/2))*sinf((initial_angle*PI) / 180)) : abs((offset + (size.x / 2))*sinf((initial_angle*PI) / 180));
		c.SetPos(position.x - ((offset + (size.x/2))*cosf((initial_angle*PI) / 180)), position.y, position.z + z_offset);
		c.SetRotation(initial_angle, { 0,1,0 });
		PhysBody3D* pbody = App->physics->AddBody(c, 0);
		curve->bodies.PushBack(pbody);
		initial_angle += increment;
	}

	return curve;
}