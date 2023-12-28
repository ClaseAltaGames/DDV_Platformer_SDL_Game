
#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "WEnemies.h"
#include "FEnemies.h"
#include "Jabon.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();	
	
	//
	bool LoadState(pugi::xml_node node);

	//
	bool SaveState(pugi::xml_node node);

	iPoint GetPlayerPosition() {
		return player->position;
	}

	iPoint GetWenemyPosition() {
		return wenemy->position;
	}

	int GetPlayerLivesAlive()
	{
		return player->lives = 1;	
	}

	int GetPlayerDeath()
	{
		return player->lives = 0;
	}
	int GetEnemyDeathScore()
	{
		return player->score += 150;
	}
private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	Player* player;
	WEnemies* wenemy;
	FEnemies* fenemy;
	List<WEnemies*> wenemyList;
	List<FEnemies*> fenemyList;

	SDL_Texture* fondo;


public:
	SDL_Texture* mouseTileTex = nullptr;
	bool pause;
	PhysBody* pbody;
};

#endif // __SCENE_H__