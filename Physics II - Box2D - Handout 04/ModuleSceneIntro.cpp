#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	/*if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}*/

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 10));
		circles.getLast()->data->listener = this;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{

		circles.add(App->physics->CreateCircle(360, 530, 8));
		circles.getLast()->data->listener = this;
		if (i != 0)
		{
			circles.getLast()->prev->data->body->SetTransform({ PIXEL_TO_METERS(10000), PIXEL_TO_METERS(10000) }, 0);
			circles.del(circles.getLast()->prev);
		}

		i++;
		
	}


	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		b2Vec2 vel = b2Vec2(0, -100);
		circles.getLast()->data->body->ApplyForceToCenter(vel, true);
	}


	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	}

		// Pivot 0, 0
		//int Pinball[166] = {
		//	3, 3,
		//	384, 3,
		//	383, 609,
		//	232, 609,
		//	230, 595,
		//	332, 522,
		//	339, 514,
		//	344, 504,
		//	344, 412,
		//	318, 392,
		//	318, 374,
		//	321, 382,
		//	326, 389,
		//	336, 390,
		//	343, 384,
		//	344, 362,
		//	336, 353,
		//	348, 344,
		//	349, 556,
		//	350, 562,
		//	355, 566,
		//	361, 567,
		//	366, 565,
		//	371, 560,
		//	371, 126,
		//	368, 108,
		//	362, 93,
		//	355, 81,
		//	365, 76,
		//	371, 69,
		//	375, 62,
		//	378, 55,
		//	379, 46,
		//	378, 36,
		//	375, 27,
		//	366, 16,
		//	360, 11,
		//	350, 7,
		//	340, 6,
		//	328, 7,
		//	319, 12,
		//	312, 18,
		//	307, 25,
		//	304, 32,
		//	302, 43,
		//	279, 38,
		//	256, 34,
		//	230, 31,
		//	217, 31,
		//	158, 32,
		//	131, 38,
		//	116, 44,
		//	100, 51,
		//	85, 61,
		//	70, 72,
		//	59, 83,
		//	51, 92,
		//	44, 101,
		//	36, 115,
		//	30, 126,
		//	25, 139,
		//	21, 152,
		//	18, 163,
		//	16, 171,
		//	15, 182,
		//	15, 294,
		//	18, 300,
		//	23, 303,
		//	30, 303,
		//	36, 300,
		//	38, 293,
		//	45, 291,
		//	14, 349,
		//	14, 358,
		//	28, 375,
		//	17, 389,
		//	17, 501,
		//	20, 511,
		//	27, 521,
		//	129, 595,
		//	128, 609,
		//	1, 609,
		//	3, 10
		//};

		//ricks.add(App->physics->CreateChain(0, 0, Pinball, 64));

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
