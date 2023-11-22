#include "FEnemies.h"
#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Window.h"

FEnemies::FEnemies() : Entity(EntityType::FENEMIES)
{
	name.Create("FEnemies");
}

FEnemies::~FEnemies() {

}

void FEnemies::FEnemiesStartAnims()
{

}


bool FEnemies::Awake()
{

	return true;
}

bool FEnemies::Start() {

	return true;
}

bool FEnemies::Update(float dt)
{

	return true;
}



bool FEnemies::CleanUp()
{

	return true;
}



void FEnemies::OnCollision(PhysBody* physA, PhysBody* physB)
{
	/*switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		currentAnimation = &idleR;
		jumpsAvaiable = 1;
		app->scene->pause = false;
		death = false;
		break;
	case ColliderType::DEATH:
		LOG("Collision DEATH");
		death = true;
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}*/
}