#pragma once

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"
#include "Animation.h"
#include "EntityManager.h"
#include "Player.h"

#include "Box2D/Box2D/Box2D.h"

struct SDL_Texture;


class Poti : public Entity
{
public:

	Poti();

	virtual ~Poti();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);


public:

	SDL_Texture* potiTex;

	PhysBody* potibody;

	bool isPicked = false;

};

