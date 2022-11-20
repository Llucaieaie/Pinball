#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct Flipper
{
	PhysBody* Rect;
	PhysBody* Circle;
	bool side;
};

class ModuleFlippers : public Module
{
public:
	ModuleFlippers(Application* app, bool start_enabled = true);
	virtual ~ModuleFlippers();

	bool Start();
	update_status Update();
	bool CleanUp();

	p2List<Flipper*> flippers;

	float angle;
};