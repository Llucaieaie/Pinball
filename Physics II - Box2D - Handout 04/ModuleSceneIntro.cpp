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
	//circle = box = rick = NULL;
	ray_on = false;
	//sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 5, SCREEN_WIDTH / 2, 5);
	bool sensed = false;

	// TITLE SCREEN
	currentScene = TITLESCREEN;
	backgroundTexture = App->textures->Load("pinball/intro.png");
	App->audio->PlayMusic("Game/pinball/bonus.wav", 0);
	startTitle = false;

	// GAME OVER SCREEN
	gameovertexture = App->textures->Load("pinball/GAMEOVER.png");


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	//TITLE SCREEN
	App->textures->Unload(backgroundTexture);

	return true;
}

update_status ModuleSceneIntro::PreUpdate() 
{
	
	return UPDATE_CONTINUE;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{

	switch (currentScene)
	{
	case TITLESCREEN:
		if (startTitle)
		{
			startTitle = false;
			App->audio->PlayMusic("Game/pinball/bonus.wav", 0.0f);
		}
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_STATE::KEY_DOWN) {
			currentScene = PINBALL;
			App->audio->PlayMusic("Game/pinball/audio/music/silence.ogg");
		}

		App->renderer->Blit(backgroundTexture, 0, 0, true);
		break;


	case PINBALL:
		{
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {

				circles.add(App->physics->CreateCircle(360, 530, 8, b2_dynamicBody));
				circles.getLast()->data->listener = this;
				if (i != 0)
				{
					circles.getLast()->prev->data->body->SetTransform({ PIXEL_TO_METERS(10000), PIXEL_TO_METERS(10000) }, 0);
					circles.del(circles.getLast()->prev);
				}
				i++;
			}	

			/*if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
			{
				currentScene = GAMEOVER;
			}*/


			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				b2Vec2 vel = b2Vec2(0, -100);
				circles.getLast()->data->body->ApplyForceToCenter(vel, true);
			}

			// Prepare for raycast ------------------------------------------------------

			iPoint mouse;
			mouse.x = App->input->GetMouseX();
			mouse.y = App->input->GetMouseY();
			int ray_hit = ray.DistanceTo(mouse);

			fVector normal(0.0f, 0.0f);

			// All draw functions ------------------------------------------------------
			p2List_item<PhysBody*>* c = circles.getFirst();


			// ray -----------------
			if (ray_on == true)
			{
				fVector destination(mouse.x - ray.x, mouse.y - ray.y);
				destination.Normalize();
				destination *= ray_hit;

				App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

				if (normal.x != 0.0f)
					App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
			}

			
		}
	case GAMEOVER:
		{
			if (currentScene == GAMEOVER) 
			{
				App->renderer->Blit(gameovertexture, 0, 0, true);
				LOG("HAS PERDIDO");

				if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_STATE::KEY_DOWN)
				{
					//App->physics->debug = false;
					currentScene = TITLESCREEN;
				}
			}
		
			
		}
		break;
	default:
		return UPDATE_ERROR;
		break;
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	if (bodyA->body == circles.getLast()->data->body && bodyB->body == sensor->body)
	{
		currentScene = GAMEOVER;
	}

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
