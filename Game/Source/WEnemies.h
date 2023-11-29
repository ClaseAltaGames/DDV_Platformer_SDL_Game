#ifndef __WENEMIES_H__
#define __WENEMIES_H__


#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "List.h"

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
	
	void WEnemiesStartAnims();

	//iPoint GetOrigin() const override;

public:
	SDL_Texture* enemyTex1 = NULL;

	Animation* playerR;
	Animation* playerL;

};

#endif