#pragma once
#include "Module.h"
#include "p2List.h"
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

public:

	SDL_Texture* flippertexture;

	p2List<Flipper*> flippers;
	SDL_Rect rectSect = { 2, 38, 51, 15 };
	

	float angle;
};