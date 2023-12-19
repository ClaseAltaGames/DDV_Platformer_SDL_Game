#pragma once


#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"
#include "Animation.h"
#include "App.h"

#include "Box2D/Box2D/Box2D.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	/*bool DeathUpdate();*/

	bool CleanUp();

	void PlayerStartAnims();

	Animation* GetAnimation(SString name);

	void OnCollision(PhysBody* physA, PhysBody* physB);



public:
	float speed = 0.12f;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;
	int pickCoinFxId;
	uint texW, texH;
	float cameraSpeed = 1.0f;
	bool death = false;
	bool godMode = false;
	int saltoFX;
	int lives = 1;
	
private:
	float jumpPower = 0.45f;
	float acceleration = 0.2f;
	uint jumpsAvaiable = 1;
	int deathAvailable = 1;
	

	Animation* currentAnimation = nullptr;

	// A set of animations
	Animation* playerR;
	Animation* playerL;
	Animation* jumpR;
	Animation* idleR;
	Animation* deathR;

	float gravityScale;

	List<Animation*> animationList;	
};


