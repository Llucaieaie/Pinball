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
	PhysBody* suma;
	PhysBody* suma2;
	PhysBody* suma3;
	
	SDL_Rect bonus1;
	SDL_Rect bonus2;
	SDL_Rect bonus3;

	SDL_Rect flecha;

	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

	int i = 0;
	//b detecta si la bola está al inicio
	bool b = true;
	int power = -5;
	int vidas = 3;

	//posicion inicial
	int inPosX;
	int inPosY;

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
	SDL_Rect spring;
	//SDL_Rect flipper;
	//SDL_Texture* flippers;
	
	


	//GAMEOVER
	SDL_Texture* gameovertexture = nullptr;
	unsigned int gameover;

	//FONTS
	int scoreFont = -1;
	int highscoreFont = -1;
	int sizescoreFont = 32;
	
};
