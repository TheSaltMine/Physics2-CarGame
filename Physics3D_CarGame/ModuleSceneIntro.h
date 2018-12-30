#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define CAMERA_Y_OFFSET 7
#define CAMERA_MIN_DISTANCE 15
#define CAMERA_MAX_DISTANCE 18
#define CAMERA_MIN_SPEED 80
#define CAMERA_MAX_SPEED 150

struct PhysBody3D;
struct Obstacle;
struct Ramp;
struct Pendulum;
struct Curve;
struct Arch;
struct Road;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	void CreateCurve(vec3 position, vec3 size, float initial_angle, float final_angle, Color color = White);
	Obstacle* CreatePillar(vec3 position, float radius, float height, float angle = 0.0f, vec3 axis = {0,0,1}, Color color = Maroon);

public:
	p2DynArray<Obstacle*> obstacles;
	Timer game_timer;

	//sensors
	p2DynArray<PhysBody3D*> sensors;
	PhysBody3D* start;
	PhysBody3D* finish;
	PhysBody3D* current_checkpoint = nullptr;

	bool debug_mode = false;
	float camera_distance = CAMERA_MIN_DISTANCE;
};
