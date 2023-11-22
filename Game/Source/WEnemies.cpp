#include "WEnemies.h"
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
WEnemies::WEnemies() : Entity(EntityType::WENEMIES)
{
	name.Create("WEnemies");
}

WEnemies::~WEnemies() {

}

void WEnemies::WEnemiesStartAnims()
{
	
}


bool WEnemies::Awake()
{
	
	return true;
}

bool WEnemies::Start() {

	return true;
}

bool WEnemies::Update(float dt)
{
	
	return true;
}



bool WEnemies::CleanUp()
{

	return true;
}



void WEnemies::OnCollision(PhysBody* physA, PhysBody* physB)
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