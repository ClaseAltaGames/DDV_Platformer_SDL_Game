#ifndef __WENEMIES_H__
#define __WENEMIES_H__


#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"
#include "Animation.h"

#include "Box2D/Box2D/Box2D.h"

struct SDL_Texture;

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

	//iPoint GetOrigin() const override;

public:

	float speed = 0.04f;

	SDL_Texture* enemyTex1 = NULL;

	PhysBody* ebody;

	Animation* currentAnimation = nullptr;

	Animation* enemy1WalkAnimR;
	Animation* enemy1WalkAnimL;


	List<Animation*> animationList;

private:
	float acceleration = 0.2f;
};

#endif