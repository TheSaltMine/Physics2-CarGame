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
	obstacles.PushBack(new Arch({ 0,50, 0 }, { 25, 5 ,5 }));
	obstacles.PushBack(new Road({ 0,50,0 }, { 25,1,50 }));
	CreateCurve({ -27.5,50,30 }, { 25,1,10 }, 90, 180);
	obstacles.PushBack(new Road({ -80,50,57.5 }, { 25,1,100 },90));
	CreateCurve({ -135,50,85 }, { 25,1,10 }, 270, 360);
	obstacles.PushBack(new Road({ -162.5,50,165 }, { 25,1,150 }));
	obstacles.PushBack(new Ramp({ -162.5, 220, 230 }, { 25, 1, 15 }, 170, 2, 1));
	obstacles.PushBack(new Ramp({ -162.5, 220, 355 }, { 25, 1, 15 }, 170, 2, 0));
	obstacles.PushBack(new Road({ -162.5,50, 370 }, { 25,1,50 }));
	CreateCurve({ -190,50,400 }, { 25,1,10 }, 90, 180);
	obstacles.PushBack(new Road({ -220,50, 427.5 }, { 25,1,50 }, 90));
	obstacles.PushBack(CreatePillar({ -220,55, 427.5 }, 2.5, 10, 90));
	CreateCurve({ -250,50,400 }, { 25,1,10 }, 0, 90);
	obstacles.PushBack(new Road({ -277.5,50, 360 }, { 25,1,75 }));
	CreateCurve({ -305,50,317.5 }, { 25,1,10 }, 180, 360);
	CreateCurve({ -360,50,327.5 }, { 25,1,10 }, 0, 180);
	CreateCurve({ -415,50,317.5 }, { 25,1,10 }, 180, 360);
	obstacles.PushBack(new Road({ -442.5,50, 360 }, { 25,1,75 }));
	obstacles.PushBack(new Ramp({ -442, 100, 395 }, { 25, 1, 5 }, 50, 72, 1, false, true));
	obstacles.PushBack(new Arch({ -478,50, 447.5 }, { 25, 5 , 5 }));
	obstacles.PushBack(new Road({ -478,50, 447.5 }, { 25,1,100 }));
	CreateCurve({ -505.5,50,500 }, { 25,1,10 }, 90, 180);
	obstacles.PushBack(new Road({ -548,50, 527.5 }, { 25,1,75 }, 90));
	obstacles.PushBack(CreatePillar({ -562,57, 527.5 }, 2.5, 15, 90));
	CreateCurve({ -590.5,50,555 }, { 25,1,10 }, 270, 360);
	obstacles.PushBack(new Road({ -618,50, 585 }, { 25,1,50 }));
	CreateCurve({ -590.5,50,615 }, { 25,1,10 }, 0, 90);
	CreateCurve({ -590.5,50,615 }, { 25,1,10 }, 90, 180);
	obstacles.PushBack(new Road({ -563,50, 598 }, { 25,1,25 }));
	obstacles.PushBack(new Ramp({ -563, 140, 588.5 }, { 25, 1, 10 }, 90, 6, 0));
	obstacles.PushBack(new Road({ -563,64.5, 482 }, { 25,1,115 }));
	CreateCurve({ -535.5,64.5, 419.5 }, { 25,1,10 }, 270, 360);
	obstacles.PushBack(new Ramp({ -538, 244.5, 392}, { 17, 1, 25 }, 180, 3, 0, true));
	obstacles.PushBack(new Road({ -375, 75, 392 }, { 25,1,75 }, 90));
	obstacles.PushBack(new Ramp({ -335, 100, 399 }, { 10, 1, 42 }, 25, 288, 0, true, true));
	obstacles.PushBack(new Road({ -292.5, 75, 550.5 }, { 25,1,75 }, 90));
	obstacles.PushBack(new Arch({ -292.5, 75, 550.5 }, { 25, 5 ,5 }, 90));
	CreateCurve({ -250, 75, 523 }, { 25,1,10 }, 90, 180);
	obstacles.PushBack(new Road({ -222.5, 75, 460 }, { 25, 1, 125 }));
	obstacles.PushBack(new Pendulum({ -222.5,101, 435 }, { 25,5,5 }, Blue, true));
	obstacles.PushBack(new Pendulum({ -222.5,101, 455 }, { 25,5,5 }, Blue));
	obstacles.PushBack(new Pendulum({ -222.5,101, 475 }, { 25,5,5 }, Blue, true));
	CreateCurve({ -195, 75, 393 }, { 25,1,10 }, 270, 360);
	obstacles.PushBack(new Road({ -140, 75, 365.5 }, { 10, 1, 100 }, 90));
	CreateCurve({ -85, 75, 393 }, { 25,1,10 }, 180, 270);
	obstacles.PushBack(new Road({ -57.5, 75, 435 }, { 25, 1, 75 }));
	CreateCurve({ -85, 75, 475 }, { 25,1,10 }, 90, 180);
	obstacles.PushBack(new Road({ -127.5, 75, 502.5 }, { 25, 1, 75 }, 90));
	obstacles.PushBack(new Ramp({ -155, 255,502.5 }, { 18, 1, 25 }, 180, 7, 1, true));
	obstacles.PushBack(new Road({ -430, 125, 500 }, { 25, 1, 75 }, 90));
	obstacles.PushBack(new Arch({ -460, 125, 500 }, { 25, 5 ,5 }, 90, Red));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	for (int i = 0; i < obstacles.Count(); i++)
	{
		delete obstacles[i];
		obstacles[i] = nullptr;
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

void ModuleSceneIntro::CreateCurve(vec3 position, vec3 size, float initial_angle, float final_angle, Color color)
{
	Cube c(size.x, size.y, size.z);
	float increment = 5.f;
	float offset = 15.f;

	while (initial_angle <= final_angle)
	{
		float z_offset = initial_angle > 180 ? -abs((offset + (size.x/2))*sinf(initial_angle*DEGTORAD)) : abs((offset + (size.x / 2))*sinf(initial_angle*DEGTORAD));
		c.SetPos(position.x - ((offset + (size.x/2))*cosf(initial_angle*DEGTORAD)), position.y, position.z + z_offset);
		c.SetRotation(initial_angle, { 0,1,0 });
		obstacles.PushBack(new Road(c.GetPos(), size, initial_angle, color));

		initial_angle += increment;
	}
}
Obstacle* ModuleSceneIntro::CreatePillar(vec3 position, float radius, float height, float angle, vec3 axis, Color color)
{
	Obstacle* pillar = new Obstacle();

	Cylinder* primitive = new Cylinder(radius, height);
	primitive->SetPos(position.x, position.y, position.z);
	primitive->SetRotation(angle, { 0,0,1 });
	primitive->color = color;

	pillar->shape = primitive;
	pillar->bodies.PushBack(App->physics->AddBody(*primitive, 0));

	return pillar;
}