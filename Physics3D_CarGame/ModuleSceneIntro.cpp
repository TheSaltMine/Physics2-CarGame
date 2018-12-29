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


	obstacles.PushBack(CreateObstacle({ 0,50,0 }, { 25,1,50 }));
	obstacles.PushBack(CreateCurve({ -27.5,50,30 }, { 25,1,10 }, 90, 180));
	obstacles.PushBack(CreateObstacle({ -80,50,57.5 }, { 100,1,25 }));
	obstacles.PushBack(CreateCurve({ -135,50,85 }, { 25,1,10 }, 270, 360));
	obstacles.PushBack(CreateObstacle({ -162.5,50,165 }, { 25,1,150 }));
	obstacles.PushBack(CreateRamp({ -162.5, 220, 230 }, { 25, 1, 15 }, 170, 2, 1));
	obstacles.PushBack(CreateRamp({ -162.5, 220, 355 }, { 25, 1, 15 }, 170, 2, 0));
	obstacles.PushBack(CreateObstacle({ -162.5,50, 370 }, { 25,1,50 }));
	obstacles.PushBack(CreateCurve({ -190,50,400 }, { 25,1,10 }, 90, 180));
	obstacles.PushBack(CreateObstacle({ -220,50, 427.5 }, { 50,1,25 }));
	obstacles.PushBack(CreateCurve({ -250,50,400 }, { 25,1,10 }, 0, 90));
	obstacles.PushBack(CreateObstacle({ -277.5,50, 360 }, { 25,1,75 }));
	obstacles.PushBack(CreateCurve({ -305,50,317.5 }, { 25,1,10 }, 180, 360));
	obstacles.PushBack(CreateCurve({ -360,50,327.5 }, { 25,1,10 }, 0, 180));
	obstacles.PushBack(CreateCurve({ -415,50,317.5 }, { 25,1,10 }, 180, 360));
	obstacles.PushBack(CreateObstacle({ -442.5,50, 360 }, { 25,1,75 }));
	obstacles.PushBack(CreateRamp({ -442, 100, 395 }, { 25, 1, 5 }, 50, 72, 1, false, true));
	obstacles.PushBack(CreateObstacle({ -478,50, 447.5 }, { 25,1,100 }));
	obstacles.PushBack(CreateCurve({ -505.5,50,500 }, { 25,1,10 }, 90, 180));
	obstacles.PushBack(CreateObstacle({ -548,50, 527.5 }, { 75,1,25 }));
	obstacles.PushBack(CreateCurve({ -590.5,50,555 }, { 25,1,10 }, 270, 360));
	obstacles.PushBack(CreateObstacle({ -618,50, 585 }, { 25,1,50 }));
	obstacles.PushBack(CreateCurve({ -590.5,50,615 }, { 25,1,10 }, 0, 90));
	obstacles.PushBack(CreateCurve({ -590.5,50,615 }, { 25,1,10 }, 90, 180));
	obstacles.PushBack(CreateObstacle({ -563,50, 598 }, { 25,1,25 }));
	obstacles.PushBack(CreateRamp({ -563, 140, 588.5 }, { 25, 1, 10 }, 90, 6, 0));
	obstacles.PushBack(CreateObstacle({ -563,64.5, 482 }, { 25,1,115 }));
	obstacles.PushBack(CreateCurve({ -535.5,64.5, 419.5 }, { 25,1,10 }, 270, 360));
	obstacles.PushBack(CreateRamp({ -538, 244.5, 392}, { 17, 1, 25 }, 180, 3, 0, true));
	obstacles.PushBack(CreateObstacle({ -375, 75, 392 }, { 75,1,25 }));
	obstacles.PushBack(CreateRamp({ -335, 105, 399 }, { 10, 1, 50 }, 30, 288, 0, true, true));
	obstacles.PushBack(CreateObstacle({ -292.5, 75, 550.5 }, { 75,1,25 }));
	obstacles.PushBack(CreateCurve({ -250, 75, 523 }, { 25,1,10 }, 90, 180));
	obstacles.PushBack(CreateObstacle({ -222.5, 75, 460 }, { 25, 1, 125 }));
	obstacles.PushBack(CreatePendulum({ -222.5,101, 435 }, { 25,5,5 }, Blue, true));
	obstacles.PushBack(CreatePendulum({ -222.5,101, 455 }, { 25,5,5 }, Blue));
	obstacles.PushBack(CreatePendulum({ -222.5,101, 475 }, { 25,5,5 }, Blue, true));

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
		vec3 back_vector = App->player->vehicle->GetBackwardVector();
		vec3 up_vector = App->player->vehicle->GetUpwardVector();

		float distance_value = (App->player->vehicle->GetKmh()-CAMERA_MIN_SPEED) / ((CAMERA_MAX_SPEED)- CAMERA_MIN_SPEED); // normalize player speed
		camera_distance = max(CAMERA_MIN_DISTANCE,((1 - distance_value)*CAMERA_MIN_DISTANCE) + (distance_value  * CAMERA_MAX_DISTANCE)); //lerp camera distance with normalized player speed

		back_vector *= camera_distance;
		up_vector *= CAMERA_Y_OFFSET;

		vec3 camera_vector = up_vector + back_vector;
		App->camera->Position = camera_vector + App->player->vehicle->GetPos();
		App->camera->Reference = camera_vector + App->player->vehicle->GetPos();

		//Camera look at player
		vec3 vehicle_pos = App->player->vehicle->GetPos();
		App->camera->LookAt(vehicle_pos+up_vector);
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

Ramp* ModuleSceneIntro::CreateRamp(vec3 position, vec3 size, int radius, int chunks, int dir, bool horizontal, bool loop, Color color)
{
	Cube c = {size.x, size.y, size.z};
	c.color = color;
	Ramp* ramp = new Ramp();
	ramp->shape = c;
	ramp->bodies = new PhysBody3D*[chunks];
	ramp->chunks = chunks;

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
			c.SetRotation(angle, { 1, 0, 0 });
			next_position = rotate(circle_center - position, angle, { 1, 0, 0 });
		}
		else
		{
			c.SetRotation(angle, { 0, 0, 1 });
			next_position = rotate(circle_center - position, angle, { 0, 0, 1 });
		}
		
		if (loop)
		{
			if(!horizontal)
				c.SetPos(next_position.x + position.x + loopincr, next_position.y + position.y, next_position.z + position.z);
			else
				c.SetPos(next_position.x + position.x, next_position.y + position.y, next_position.z + position.z + loopincr);
		}
		else
			c.SetPos(next_position.x + position.x, next_position.y + position.y, next_position.z + position.z);

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