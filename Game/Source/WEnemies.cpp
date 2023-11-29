#pragma once

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
	for (pugi::xml_node animNode = parameters.child("animation"); animNode; animNode = animNode.next_sibling())
	{
		Animation* anim = new Animation();

		anim->name = animNode.attribute("name").as_string();
		anim->speed = animNode.attribute("speed").as_float();
		anim->loop = animNode.attribute("loop").as_bool();
		anim->pingpong = animNode.attribute("pingpong").as_bool();

		for (pugi::xml_node frameNode = animNode.child("frame"); frameNode; frameNode = frameNode.next_sibling())
		{
			int x = frameNode.attribute("x").as_int();
			int y = frameNode.attribute("y").as_int();
			int w = frameNode.attribute("w").as_int();
			int h = frameNode.attribute("h").as_int();
			anim->PushBack({ x,y,w,h });
		}
		animationList.Add(anim);
	}

	
	playerR = GetAnimation("enemy1WalkAnimR");
	playerL = GetAnimation("enemy1WalkAnimL");
	
}


bool WEnemies::Awake()
{
	position = iPoint(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());

	return true;
}

bool WEnemies::Start() {

	enemyTex1 = app->tex->Load(parameters.attribute("texturepath").as_string());

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
//void WEnemies::WEnemiesStartAnims()	
//{
//	for (pugi::xml_node animNode = parameters.child("animation"); animNode; animNode = animNode.next_sibling())
//	{
//		Animation* anim = new Animation();
//
//		anim->name = animNode.attribute("name").as_string();
//		anim->speed = animNode.attribute("speed").as_float();
//		anim->loop = animNode.attribute("loop").as_bool();
//		anim->pingpong = animNode.attribute("pingpong").as_bool();
//
//		for (pugi::xml_node frameNode = animNode.child("frame"); frameNode; frameNode = frameNode.next_sibling())
//		{
//			int x = frameNode.attribute("x").as_int();
//			int y = frameNode.attribute("y").as_int();
//			int w = frameNode.attribute("w").as_int();
//			int h = frameNode.attribute("h").as_int();
//			anim->PushBack({ x,y,w,h });
//		}
//		animationList.Add(anim);
//	}
//
//	idleAnim = GetAnimation("idle");
//	forwardAnim = GetAnimation("forwardAnim");
//	backwardAnim = GetAnimation("backwardAnim");
//	forwardjump = GetAnimation("forwardJump");
//	backwardjump = GetAnimation("backwardJump");
//	death = GetAnimation("death");
//
//
//}


//Animation* WEnemies::GetAnimation(SString name)
//{
//	for (ListItem<Animation*>* item = animationList.start; item != nullptr; item = item->next)
//	{
//		if (item->data != nullptr) {
//			if (item->data->name == name) return item->data;
//		}
//	}
//	return nullptr;
//}
