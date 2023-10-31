#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

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

	bool PostUpdate();

	bool CleanUp();

	void PlayerStartAnims();

	

	void OnCollision(PhysBody* physA, PhysBody* physB);


public:
	float speed = 0.12f;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;
	int pickCoinFxId;
	uint texW, texH;
	float cameraSpeed = 1.0f;

private:
	float jumpPower = 0.5f;
	float acceleration = 0.2f;
	uint jumpsAvaiable = 1;
	

	Animation* currentAnimation = nullptr;

	// A set of animations
	Animation playerR, playerL,
		punchR, punchL,
		deathR, deathL,
		hitR, hitL,
		idleR, idleL,
		jumpR, jumpL,
		fallR, fallL,
		pushR, pushL,
		runR, runL,
		throwR, throwL,
		walkR, walkL,
		walkPunchR, walkPunchL;
};


#endif // __PLAYER_H__