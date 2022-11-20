#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct Flipper
{
	PhysBody* Rect;
	PhysBody* Circle;
	bool rightSide;

	p2List<Flipper*> flippers;
};

class ModuleFlippers : public Module
{
public:
	ModuleFlippers(Application* app, bool start_enabled = true);
	virtual ~ModuleFlippers();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

};