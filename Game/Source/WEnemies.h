#ifndef __WENEMIES_H__
#define __WENEMIES_H__


#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"
#include "Animation.h"
#include "EntityManager.h"
#include "Player.h"

#include "Box2D/Box2D/Box2D.h"

struct SDL_Texture;

enum class EnemyState
{
	MOVING_TO_DESTINATION,
	MOVING_TO_ORIGIN
};

class WEnemies : public Entity
{
public:

	WEnemies();

	virtual ~WEnemies();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	Animation* GetAnimation(SString name);
	
	void WEnemiesStartAnims();

	void MoveToDestination(float dt);

	void MoveToOrigin(float dt);

	bool HasReachedDestination();

	bool HasReachedOrigin();


	//iPoint GetOrigin() const override;

public:

	float speed = 0.04f;

	SDL_Texture* enemyTex1 = NULL;

	PhysBody* ebody; 
	PhysBody* deathBody;

	Animation* currentAnimation = nullptr;

	Animation* enemy1WalkAnimR;
	Animation* enemy1WalkAnimL;

	EnemyState currentState;

	float acceleration = 0.2f;

	List<Animation*> animationList;

	Player* player;

};

#endif