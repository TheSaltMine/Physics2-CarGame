#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2.5, 0.5, 5);
	car.chassis_offset.Set(0, 1, 0);

	car.chassis2_size.Set(3.5, 0.5, 0.5);
	car.chassis2_offset.Set(0, 1, 2.5);

	car.chassis3_size.Set(3.5, 0.5, 0.5);
	car.chassis3_offset.Set(0, 1, -2.5);

	/*car.head_size.Set(0.5, 1, 1);
	car.head_offset.Set(0, 2, 0);*/

	car.body_size.Set(2, 0.5, 3.5);
	car.body_offset.Set(0, 1.5, 0);

	car.mass = 300.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 100;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 0.3f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.75f;
	float half_length = car.chassis_size.z*0.62f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 50, 0);
	vehicle->collision_listeners.add(App->scene_intro);
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

PhysVehicle3D * ModulePlayer::GetVehicle()
{
	return vehicle;
}

void ModulePlayer::Stop()
{
	vehicle->SetVelocity(0,0,0);
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	if (can_move)
	{
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && vehicle->GetKmh() < MAX_VELOCITY)
		{
			acceleration = MAX_ACCELERATION;
		}

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			if (turn < TURN_DEGREES)
				turn += TURN_DEGREES;
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			if (turn > -TURN_DEGREES)
				turn -= TURN_DEGREES;
		}

		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			if (vehicle->GetKmh() <= 0)
				acceleration = -MAX_ACCELERATION;
			else
				brake = BRAKE_POWER;
		}
	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();

	return UPDATE_CONTINUE;
}



