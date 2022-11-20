#include "Globals.h"
#include "Application.h"
#include "Flippers.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"


ModuleFlippers::ModuleFlippers(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleFlippers::~ModuleFlippers()
{}

// Load assets
bool ModuleFlippers::Start()
{
	LOG("Loading flippers");

	b2Vec2 a = { -0.44, 0 };
	b2Vec2 b = { 0, 0 };

	Flipper* f = new Flipper;
	f->Circle = App->physics->CreateCircle(124, 555, 5, b2_staticBody);
	f->Rect = App->physics->CreateRectangle(124, 555+5, 50, 10, b2_dynamicBody);
	f->side = false;
	App->physics->CreateRevoluteJoint(f->Rect, a, f->Circle, b, 35.0f);
	flippers.add(f);


	Flipper* f2 = new Flipper;
	f2->Circle = App->physics->CreateCircle(237, 555, 5, b2_staticBody);
	f2->Rect = App->physics->CreateRectangle(227-50, 545 + 5, 50, 10, b2_dynamicBody);
	f2->side = true;
	App->physics->CreateRevoluteJoint(f2->Rect, -a, f2->Circle, -b, 35.0f);
	flippers.add(f2);
	return true;
}

// Unload assets
bool ModuleFlippers::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModuleFlippers::Update()
{
	//Flippers --------------------------------------------------
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_REPEAT)
	{
		p2List_item<Flipper*>* f = flippers.getFirst();
		while (f != NULL)
		{
			if (f->data->side == false)
			{
				f->data->Rect->body->ApplyForce({ -3,0 }, { 0,0 }, true);
			}
			f = f->next;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_REPEAT)
	{
		p2List_item<Flipper*>* f = flippers.getFirst();
		while (f != NULL)
		{
			if (f->data->side == true)
			{
				f->data->Rect->body->ApplyForce({ 3,0 }, { 0,0 }, true);
			}
			f = f->next;
		}
	}
	return UPDATE_CONTINUE;
}