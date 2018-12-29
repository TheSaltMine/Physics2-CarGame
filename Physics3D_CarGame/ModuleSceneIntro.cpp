#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"
#include "Obstacle.h"
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

	App->audio->PlayMusic("audio/music.ogg");
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	game_timer.Start();

	//Sensors
	PhysBody3D* tmp_sensor;
	Cube sensor(25, 5, 5);
	sensor.SetPos(0, 50, 0);
	start = App->physics->AddBody(sensor, 0);
	start->SetAsSensor(true);
	sensors.PushBack(start);
	sensor.SetPos(-478, 50, 447.5);
	tmp_sensor = App->physics->AddBody(sensor, 0);
	tmp_sensor->SetAsSensor(true);
	sensors.PushBack(tmp_sensor);

	sensor.SetRotation(90, { 0,1,0 });
	sensor.SetPos(-460, 125, 500);
	finish = App->physics->AddBody(sensor, 0);
	finish->SetAsSensor(true);
	sensors.PushBack(finish);
	sensor.SetPos(-292.5, 75, 550.5);
	tmp_sensor = App->physics->AddBody(sensor, 0);
	tmp_sensor->SetAsSensor(true);
	sensors.PushBack(tmp_sensor);

	current_checkpoint = start;

	//Map obstacles
	obstacles.PushBack(CreateArch({ 0,50, 0 }, { 25, 5 ,5 }));
	obstacles.PushBack(CreateRoad({ 0,50,0 }, { 25,1,50 }));
	CreateCurve({ -27.5,50,30 }, { 25,1,10 }, 90, 180);
	obstacles.PushBack(CreateRoad({ -80,50,57.5 }, { 25,1,100 },90));
	CreateCurve({ -135,50,85 }, { 25,1,10 }, 270, 360);
	obstacles.PushBack(CreateRoad({ -162.5,50,165 }, { 25,1,150 }));
	obstacles.PushBack(CreateRamp({ -162.5, 220, 230 }, { 25, 1, 15 }, 170, 2, 1));
	obstacles.PushBack(CreateRamp({ -162.5, 220, 355 }, { 25, 1, 15 }, 170, 2, 0));
	obstacles.PushBack(CreateRoad({ -162.5,50, 370 }, { 25,1,50 }));
	CreateCurve({ -190,50,400 }, { 25,1,10 }, 90, 180);
	obstacles.PushBack(CreateRoad({ -220,50, 427.5 }, { 25,1,50 }, 90));
	CreateCurve({ -250,50,400 }, { 25,1,10 }, 0, 90);
	obstacles.PushBack(CreateRoad({ -277.5,50, 360 }, { 25,1,75 }));
	CreateCurve({ -305,50,317.5 }, { 25,1,10 }, 180, 360);
	CreateCurve({ -360,50,327.5 }, { 25,1,10 }, 0, 180);
	CreateCurve({ -415,50,317.5 }, { 25,1,10 }, 180, 360);
	obstacles.PushBack(CreateRoad({ -442.5,50, 360 }, { 25,1,75 }));
	obstacles.PushBack(CreateRamp({ -442, 100, 395 }, { 25, 1, 5 }, 50, 72, 1, false, true));
	obstacles.PushBack(CreateArch({ -478,50, 447.5 }, { 25, 5 , 5 }));
	obstacles.PushBack(CreateRoad({ -478,50, 447.5 }, { 25,1,100 }));
	CreateCurve({ -505.5,50,500 }, { 25,1,10 }, 90, 180);
	obstacles.PushBack(CreateRoad({ -548,50, 527.5 }, { 25,1,75 }, 90));
	CreateCurve({ -590.5,50,555 }, { 25,1,10 }, 270, 360);
	obstacles.PushBack(CreateRoad({ -618,50, 585 }, { 25,1,50 }));
	CreateCurve({ -590.5,50,615 }, { 25,1,10 }, 0, 90);
	CreateCurve({ -590.5,50,615 }, { 25,1,10 }, 90, 180);
	obstacles.PushBack(CreateRoad({ -563,50, 598 }, { 25,1,25 }));
	obstacles.PushBack(CreateRamp({ -563, 140, 588.5 }, { 25, 1, 10 }, 90, 6, 0));
	obstacles.PushBack(CreateRoad({ -563,64.5, 482 }, { 25,1,115 }));
	CreateCurve({ -535.5,64.5, 419.5 }, { 25,1,10 }, 270, 360);
	obstacles.PushBack(CreateRamp({ -538, 244.5, 392}, { 17, 1, 25 }, 180, 3, 0, true));
	obstacles.PushBack(CreateRoad({ -375, 75, 392 }, { 25,1,75 }, 90));
	obstacles.PushBack(CreateRamp({ -335, 105, 399 }, { 10, 1, 42 }, 30, 288, 0, true, true));
	obstacles.PushBack(CreateRoad({ -292.5, 75, 550.5 }, { 25,1,75 }, 90));
	obstacles.PushBack(CreateArch({ -292.5, 75, 550.5 }, { 25, 5 ,5 }, 90));
	CreateCurve({ -250, 75, 523 }, { 25,1,10 }, 90, 180);
	obstacles.PushBack(CreateRoad({ -222.5, 75, 460 }, { 25, 1, 125 }));
	obstacles.PushBack(CreatePendulum({ -222.5,101, 435 }, { 25,5,5 }, Blue, true));
	obstacles.PushBack(CreatePendulum({ -222.5,101, 455 }, { 25,5,5 }, Blue));
	obstacles.PushBack(CreatePendulum({ -222.5,101, 475 }, { 25,5,5 }, Blue, true));
	CreateCurve({ -195, 75, 393 }, { 25,1,10 }, 270, 360);
	obstacles.PushBack(CreateRoad({ -140, 75, 365.5 }, { 25, 1, 100 }, 90));
	CreateCurve({ -85, 75, 393 }, { 25,1,10 }, 180, 270);
	obstacles.PushBack(CreateRoad({ -57.5, 75, 435 }, { 25, 1, 75 }));
	CreateCurve({ -85, 75, 475 }, { 25,1,10 }, 90, 180);
	obstacles.PushBack(CreateRoad({ -127.5, 75, 502.5 }, { 25, 1, 75 }, 90));
	obstacles.PushBack(CreateRamp({ -155, 255,502.5 }, { 18, 1, 25 }, 180, 7, 1, true));
	obstacles.PushBack(CreateRoad({ -430, 125, 500 }, { 25, 1, 75 }, 90));
	obstacles.PushBack(CreateArch({ -460, 125, 500 }, { 25, 5 ,5 }, 90, Red));

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
	{
		debug_mode = !debug_mode;
		App->player->can_move = !App->player->can_move;
		App->camera->debug = !App->camera->debug;
	}
	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		mat4x4 matrix;
		current_checkpoint->GetTransform(&matrix);
		App->player->vehicle->SetTransform(&matrix);
	}
	else if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		current_checkpoint = start;
		mat4x4 matrix;
		current_checkpoint->GetTransform(&matrix);
		App->player->vehicle->SetTransform(&matrix);
		game_timer.Start();
		debug_mode = false;
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

	for (int i = 0; i < obstacles.Count(); i++)
	{
		obstacles[i]->Render();
	}


	char title[100];
	Uint32 ms = game_timer.Read();
	Uint32 seconds = ms/1000;
	Uint32 minutes = seconds/60;
	sprintf_s(title, "PLAYER_TIME: %02d:%02d:%02d   GOLD_TIME: 01:30:00    SILVER: 02:30:00    BRONZE: 05:00:00", minutes, seconds - (60*minutes), (ms % 1000)/10);
	App->window->SetTitle(title);


	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1 == App->player->vehicle)
	{
		if (body2 == finish)
		{
			//finish game
			debug_mode = true;
			game_timer.Stop();
		}
		else
		{
			for (int i = 0; i < sensors.Count(); i++)
			{
				if (sensors[i] == body2)
				{
					current_checkpoint = sensors[i];
				}
			}
		}
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
	pendulum->bodies.PushBack(App->physics->AddBody(b, 90000));
	App->physics->AddConstraintHinge(*pendulum->anchor, *pendulum->bodies[0], { 0, 0 ,0 }, { -size.x/2, 0, 0 }, { 0,0,1 }, { 0,0,1 }, true);

	return pendulum;
}

Ramp* ModuleSceneIntro::CreateRamp(vec3 position, vec3 size, int radius, int chunks, int dir, bool horizontal, bool loop, Color color)
{
	Cube c = {size.x, size.y, size.z};
	c.color = color;
	Ramp* ramp = new Ramp();
	ramp->shape = c;

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
		ramp->bodies.PushBack(pbody);
	}

	return ramp;
}

Road* ModuleSceneIntro::CreateRoad(vec3 position, vec3 size, float angle, Color color)
{
	Road* obstacle = new Road();

	Cube road(size.x, size.y, size.z);
	road.color = color;
	road.SetPos(position.x, position.y, position.z);
	road.SetRotation(angle, { 0,1,0 });
	obstacle->shape = road;
	obstacle->bodies.PushBack(App->physics->AddBody(road, 0));

	Cube border(size.x / 6, size.y, size.z);
	border.color = Gray;
	obstacle->border_shape = border;
	float z_offset = angle > 180 ? -abs((sin(angle*DEGTORAD)*(size.x / 2))) : abs((sin(angle*DEGTORAD)*(size.x / 2)));
	float x_offset = angle >90 && angle <270 ? -abs((cos(angle*DEGTORAD)*(size.x / 2))) : abs((cos(angle*DEGTORAD)*(size.x / 2)));


	border.SetRotation(angle, { 0,1,0 });
	border.SetPos(position.x - x_offset, position.y + (size.y / 2), position.z + z_offset);
	obstacle->bodies.PushBack(App->physics->AddBody(border, 0));
	border.SetPos(position.x + x_offset, position.y + (size.y / 2), position.z - z_offset);
	obstacle->bodies.PushBack(App->physics->AddBody(border, 0));



	return obstacle;
}

void ModuleSceneIntro::CreateCurve(vec3 position, vec3 size, float initial_angle, float final_angle, Color color)
{
	//Curve* curve = new Curve();
	Cube c(size.x, size.y, size.z);
	//c.color = color;
	//curve->shape = c;
	float increment = 5.f;
	float offset = 15.f;

	while (initial_angle <= final_angle)
	{
		float z_offset = initial_angle > 180 ? -abs((offset + (size.x/2))*sinf(initial_angle*DEGTORAD)) : abs((offset + (size.x / 2))*sinf(initial_angle*DEGTORAD));
		c.SetPos(position.x - ((offset + (size.x/2))*cosf(initial_angle*DEGTORAD)), position.y, position.z + z_offset);
		c.SetRotation(initial_angle, { 0,1,0 });
		obstacles.PushBack(CreateRoad(c.GetPos(), size, initial_angle, color));
		//curve->bodies.PushBack(pbody);

		initial_angle += increment;
	}

	//return curve;
}

Arch* ModuleSceneIntro::CreateArch(vec3 position, vec3 size, float angle, Color color)
{
	Arch* arch = new Arch();

	Cube base(size.x, size.y, size.z);
	base.SetPos(position.x, position.y + (size.x/2), position.z);
	base.SetRotation(angle, { 0,1,0 });
	base.color = color;
	arch->bodies.PushBack(App->physics->AddBody(base, 0));
	arch->shape = base;

	Cube c(size.y, size.x / 2, size.z);
	c.color = color;
	arch->column_shape = c;
	c.SetPos(position.x + (cos(angle*DEGTORAD)*(size.x/2)), position.y + (size.x / 4), position.z + (sin(angle*DEGTORAD)*(size.x / 2)));
	arch->bodies.PushBack(App->physics->AddBody(c, 0));
	c.SetPos(position.x - (cos(angle*DEGTORAD)*(size.x / 2)), position.y + (size.x /4), position.z - (sin(angle*DEGTORAD)*(size.x / 2)));
	arch->bodies.PushBack(App->physics->AddBody(c, 0));

	return arch;

}