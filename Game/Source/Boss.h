#ifndef __BOSS_H__
#define __BOSS_H__


#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"
#include "Animation.h"

#include "Box2D/Box2D/Box2D.h"

struct SDL_Texture;

enum class EnemyBossState
{
	MOVING_TO_DESTINATION,
	MOVING_TO_ORIGIN,
	ATTACKING,
};

class Boss : public Entity
{
public:

	Boss();

	virtual ~Boss();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	Animation* GetAnimation(SString name);

	void BossStartAnims();

	void MoveToDestination(float dt);

	void MoveToOrigin(float dt);

	bool HasReachedDestination();

	bool HasReachedOrigin();


	//iPoint GetOrigin() const override;

public:

	float speed = 0.08f;

	SDL_Texture* enemyTex1 = NULL;

	PhysBody* bossbody;

	Animation* currentAnimation = nullptr;

	Animation* bossWalkR;
	Animation* bossWalkL;

	EnemyBossState currentState;

	float acceleration = 0.2f;

	List<Animation*> animationList;

	bool death = false;
	 
	int lives = 5;

private:
	int bossDeathFx;
	SDL_Texture* boss5lives;
	SDL_Texture* boss4lives;
	SDL_Texture* boss3lives;
	SDL_Texture* boss2lives;
	SDL_Texture* boss1lives;

};

#endif