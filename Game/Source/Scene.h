
#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "WEnemies.h"
#include "FEnemies.h"
#include "Jabon.h"
#include "Health.h"
#include "Physics.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene(bool startEnabled = true);

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
		return player->lives;	
	}

	int GetPlayerDeath()
	{
		return player->lives--;
	}
	int GetWEnemyDeathScore()
	{
		return player->score += 150;
	}
	int GetFEnemyDeathScore()
	{
		return player->score += 300;
	}
	PhysBody* GoToLevel2()
	{
		app->physics->DestroyCircle(player->pbody);
		player->pbody = app->physics->CreateCircle(39 + 16, 1119 + 16, 8, bodyType::DYNAMIC);

		player->pbody->listener = player;
		player->pbody->ctype = ColliderType::PLAYER;

		return player->pbody;	
	}
	PhysBody* GoToBoss()
	{
		app->physics->DestroyCircle(player->pbody);
		player->pbody = app->physics->CreateCircle(196 + 16, 1964 + 16, 8, bodyType::DYNAMIC);

		player->pbody->listener = player;
		player->pbody->ctype = ColliderType::PLAYER;

		return player->pbody;
	}
	int CameraLevel1()
	{
		return app->render->camera.y = 0;
	}
	int CameraLevel2()
	{
		return app->render->camera.y = -1725;
	}
	int CameraBoss()
	{
		return app->render->camera.y = -3225;	
	}
	int HeartPicked()
	{
		if (player->lives < 3)
		{
			return player->lives++;
		}
	}
	bool CheckpointOFF()
	{
		return player->checkpoint = false;
	}
	PhysBody* GetPlayerPbody()
	{
		return player->pbody;
	}
	//return player module
	Player* GetPlayer()
	{
		return player;
	}

	
private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	Player* player;
	WEnemies* wenemy;
	FEnemies* fenemy;
	Jabon* item;
	Poti* poti;
	List<WEnemies*> wenemyList;
	List<FEnemies*> fenemyList;

	SDL_Texture* fondo;


public:
	SDL_Texture* mouseTileTex = nullptr;
	bool pause;
	PhysBody* pbody;

	bool level1;
	bool level2;
	bool bossLevel;
};

#endif // __SCENE_H__