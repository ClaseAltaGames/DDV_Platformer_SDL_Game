#ifndef __FENEMIES_H__
#define __FENEMIES_H__


#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"
#include "Animation.h"

#include "Box2D/Box2D/Box2D.h"

struct SDL_Texture;

enum class EnemyFlyState
{
	MOVING_TO_DESTINATION,
	MOVING_TO_ORIGIN
};

class FEnemies : public Entity
{
public:

	FEnemies();

	virtual ~FEnemies();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	Animation* GetAnimation(SString name);

	void FEnemiesStartAnims();

	void MoveToDestination(float dt);

	void MoveToOrigin(float dt);

	bool HasReachedDestination();

	bool HasReachedOrigin();


	//iPoint GetOrigin() const override;

public:

	float speed = 0.08f;

	SDL_Texture* enemyTex1 = NULL;

	PhysBody* ebody;

	Animation* currentAnimation = nullptr;

	Animation* enemy1FlyAnimR;
	Animation* enemy1FlyAnimL;

	EnemyFlyState currentState;

	float acceleration = 0.2f;

	List<Animation*> animationList;

};

#endif