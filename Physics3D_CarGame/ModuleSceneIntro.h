#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define CAMERA_Y_OFFSET 7

struct PhysBody3D;
struct Obstacle;
struct Ramp;
struct Pendulum;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	Pendulum* CreatePendulum(vec3 position, vec3 size, Color color);
	Ramp* CreateRamp(vec3 position, vec3 size, int radius, int chunks, int dir = 0, bool loop = false, Color color = Red);

public:
	p2DynArray<Obstacle*> obstacles;
	bool debug_mode = false;
};
