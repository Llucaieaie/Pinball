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


	//BONUS 2X
	suma = App->physics->CreateCircle(26, 290, 11, b2_staticBody);
	suma = App->physics->CreateCircle(185, 142, 11, b2_staticBody);
	suma = App->physics->CreateCircle(249, 154, 11, b2_staticBody);
	suma = App->physics->CreateCircle(212, 190, 11, b2_staticBody);

	//BONUS 3X
	suma2 = App->physics->CreateCircle(262, 215, 11, b2_staticBody);
	suma2 = App->physics->CreateCircle(72, 358, 11, b2_staticBody);
	suma2 = App->physics->CreateCircle(242, 365, 11, b2_staticBody);

	//BONUS 5X
	suma3 = App->physics->CreateCircle(130, 117, 11, b2_staticBody);
	suma3 = App->physics->CreateCircle(332, 377, 11, b2_staticBody);


	bool sensed = false;

	// TITLE SCREEN
	currentScene = TITLESCREEN;
	backgroundTexture = App->textures->Load("pinball/intro.png");
	App->audio->PlayMusic("Game/pinball/bonus.wav", 0);
	startTitle = false;

	//PINBALL
	background = App->textures->Load("pinball/PinballDef.png");
	assets = App->textures->Load("pinball/pinballAssets.png");
	ball = { 253, 164, 15, 15 };

	//BONUS
	bonus1 = { 135, 113, 23, 24 };
	bonus2 = { 164, 113, 23, 24 };
	bonus3 = { 192, 113, 23, 24 };


	// GAME OVER SCREEN
	gameovertexture = App->textures->Load("pinball/GAMEOVER.png");

	// FONT
	scoreFont = App->fonts->Load("pinball/nesfont1.png", " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{:}~ª", 6);
	highscoreFont = App->fonts->Load("pinball/nesfont.png", " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{:}~ª", 6);

	// BALL
	circles.add(App->physics->CreateCircle(360, 530, 8, b2_dynamicBody));
	circles.getLast()->data->listener = this;

	inPosX = circles.getLast()->data->body->GetPosition().x;
	inPosY = circles.getLast()->data->body->GetPosition().y;
	vidas = 1;
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
	LOG("vidas: %d", vidas);
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
			App->audio->PlayMusic("pinball/music.ogg");
		}

		App->renderer->Blit(backgroundTexture, 0, 0, true);
		break;


	case PINBALL:
		{

			App->renderer->Blit(background, 0, 0, NULL);

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) 
			{

			
				App->renderer->Blit(background, 0, 0, NULL);

				//BONUS 2X
				App->renderer->Blit(assets, 16, 278, false, &bonus1);
				App->renderer->Blit(assets, 173, 130, false, &bonus1);
				App->renderer->Blit(assets, 237, 142, false, &bonus1);
				App->renderer->Blit(assets, 200, 178, false, &bonus1);

				//BONUS 3X
				App->renderer->Blit(assets, 250, 203, false, &bonus2);
				App->renderer->Blit(assets, 60, 346, false, &bonus2);
				App->renderer->Blit(assets, 230, 353, false, &bonus2);

				//BONUS 5X
				App->renderer->Blit(assets, 118, 105, false, &bonus3);
				App->renderer->Blit(assets, 320, 365, false, &bonus3);


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
			//App->renderer->Blit(assets, circles.getLast()->prev->data->body->GetPosition().x, circles.getLast()->prev->data->body->GetPosition().y, false, NULL);
			//App->renderer->Blit(assets, 124, 555, false, &flipper, 1.0f, App->flippers->angle);

				if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
				{
					currentScene = GAMEOVER;
				}


				if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && b == true)
				{

					if (power > -100)
					{
						power-=5;
					}

					LOG("poder %d", power);
				}
				else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP && b == true)
				{
					b2Vec2 vel = b2Vec2(0, power);
					circles.getLast()->data->body->ApplyForceToCenter(vel, true);
					b = false;
					LOG("poder %d", power);
				}
				else power = -5;

				int x;
				int y;
				circles.getLast()->data->GetPosition(x, y);
				LOG("%d", y);
				App->renderer->Blit(assets, x, y, false, &ball);
				if (y > 800)
				{
					circles.getLast()->data->body->SetTransform({ PIXEL_TO_METERS(360), PIXEL_TO_METERS(530) }, 0);
					b = true;
					vidas--;
				}
				LOG("position.x: %d", circles.getLast()->data->body->GetPosition().x);
				LOG("position.y: %d", circles.getLast()->data->body->GetPosition().y);
				if (circles.getLast()->data->body->GetPosition().x >= inPosX && circles.getLast()->data->body->GetPosition().y >= inPosY)
					b = true;

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

			break;
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

void OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	if (bodyA->body == circles.getLast()->data->body && bodyB->body == sensor->body && vidas == 0)
	{
		currentScene = GAMEOVER;
		currentScore = 0;
<<<<<<< Updated upstream
		vidas = 2;
=======
>>>>>>> Stashed changes
	}
	
	if (bodyA->body == circles.getLast()->data->body && bodyB->body == suma->body) currentScore += 20;
	
	if (bodyA->body == circles.getLast()->data->body && bodyB->body == suma2->body) currentScore += 30;

	if (bodyA->body == circles.getLast()->data->body && bodyB->body == suma3->body) currentScore += 50;

}
