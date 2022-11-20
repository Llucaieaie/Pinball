#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "Flippers.h"

#include "ModuleFonts.h"
#include "ModulePlayer.h"


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//circle = box = rick = NULL;
	//ray_on = false;
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

	//PINBALL
	background = App->textures->Load("pinball/PinballDef.png");
	ballTexture = App->textures->Load("pinball/pinballAssets.png");

	// GAME OVER SCREEN
	gameovertexture = App->textures->Load("pinball/GAMEOVER.png");

	// FONT
	scoreFont = App->fonts->Load("pinball/nesfont1.png", " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{:}~�", 6);
	highscoreFont = App->fonts->Load("pinball/nesfont.png", " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{:}~�", 6);


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	//TITLE SCREEN
	App->textures->Unload(backgroundTexture);

	//GAME OVER
	App->textures->Unload(gameovertexture);

	//FONT
	App->fonts->Unload(scoreFont);

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
			App->physics->debug = false;
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

			App->renderer->Blit(background, 0, 0, NULL);

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {

				circles.add(App->physics->CreateCircle(360, 530, 8, b2_dynamicBody));
				circles.getLast()->data->listener = this;
				if (i != 0)
				{
					circles.getLast()->prev->data->body->SetTransform({ PIXEL_TO_METERS(10000), PIXEL_TO_METERS(10000) }, 0);
					circles.del(circles.getLast()->prev);
				}
				i++;
				App->renderer->Blit(ballTexture, circles.getLast()->prev->data->body->GetPosition().x, circles.getLast()->prev->data->body->GetPosition().y, false, NULL);
			}	

			if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
			{
				currentScene = GAMEOVER;
			}


			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				b2Vec2 vel = b2Vec2(0, -100);
				circles.getLast()->data->body->ApplyForceToCenter(vel, true);
			}

			// All draw functions ------------------------------------------------------
			p2List_item<PhysBody*>* c = circles.getFirst();

			// Text UI ----------------
			App->fonts->BlitText(sizescoreFont - 18, sizescoreFont - 15, scoreFont, "SCORE");
			sprintf_s(currentScoreNum, 12, "%6d", currentScore);
			App->fonts->BlitText(sizescoreFont - 18, sizescoreFont + 5, scoreFont, currentScoreNum);
						
			App->fonts->BlitText(sizescoreFont * 9, sizescoreFont - 15, scoreFont, "H-SCORE");
			sprintf_s(highScoreNum, 12, "%6d", highScore);
			App->fonts->BlitText(sizescoreFont + 273, sizescoreFont + 5, scoreFont, highScoreNum);

			if (currentScore > highScore) highScore = currentScore;

			if (App->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN) currentScore += 100;
		}
	case GAMEOVER:
		{
			if (currentScene == GAMEOVER) 
			{
				App->physics->debug = false;
				App->renderer->Blit(gameovertexture, 0, 0, true);
				LOG("HAS PERDIDO");

				if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_STATE::KEY_DOWN)
				{
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
		currentScore = 0;

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
