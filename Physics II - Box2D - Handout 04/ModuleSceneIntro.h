#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"


enum Scene
{
	TITLESCREEN,
	PINBALL,
	GAMEOVER
};


class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

	int i = 0;
	bool b = true;

	// Font stuff
	int font = -1;
	int fontSize = 12;

	char currentScoreNum[12] = { "\0" };
	char highScoreNum[12] = { "\0" };
	char prevScoreNum[12] = { "\0" };

	int currentScore = 0; 
	int previousScore; 
	int highScore = 0;

	//SCENES
	Scene currentScene;

	//TITLE SCREEN
	SDL_Texture* backgroundTexture = nullptr;
	bool startTitle;

	//PINBALL
	SDL_Texture* background = nullptr;
	SDL_Texture* assets;
	SDL_Rect ball;
	SDL_Rect flipper;


	//GAMEOVER
	SDL_Texture* gameovertexture = nullptr;

	//FONTS
	int scoreFont = -1;
	int highscoreFont = -1;
	int sizescoreFont = 32;

	Animation currentAnimation;
	
};
