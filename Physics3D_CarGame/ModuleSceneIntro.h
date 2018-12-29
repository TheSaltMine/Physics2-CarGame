#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define CAMERA_Y_OFFSET 7
#define CAMERA_MIN_DISTANCE 15
#define CAMERA_MAX_DISTANCE 20
#define CAMERA_MIN_SPEED 100
#define CAMERA_MAX_SPEED 150

struct PhysBody3D;
struct Obstacle;
struct Ramp;
struct Pendulum;
struct Curve;
struct Checkpoint;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	Pendulum* CreatePendulum(vec3 position, vec3 size, Color color = Blue, bool left = false);
	Ramp* CreateRamp(vec3 position, vec3 size, int radius, int chunks, int dir = 0, bool horizontal = false, bool loop = false, Color color = Red);
	Curve* CreateCurve(vec3 position, vec3 size, float initial_angle, float final_angle, Color color = White);
	Checkpoint* CreateCheckpoint(vec3 position, vec3 size, bool horizontal = true, Color color = Green);
	Obstacle* CreateObstacle(vec3 position, vec3 size, Color color = White);

public:
	p2DynArray<Obstacle*> obstacles;
	PhysBody3D* checkpoint;

	PhysBody3D* current_checkpoint = nullptr;
	bool debug_mode = false;
	float camera_distance = CAMERA_MIN_DISTANCE;
};
