#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define CAMERA_Y_OFFSET 7

struct PhysBody3D;

struct Pendulum
{
	PhysBody3D* anchor = nullptr;
	PhysBody3D* body = nullptr;
	Cube shape;
	Pendulum() {};
	void Render();
};

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

public:
	Pendulum* pendulum_1;
	Cube ramp_cube;
	p2DynArray<PhysBody3D*> ramp_1;
	bool debug_mode = false;
};
