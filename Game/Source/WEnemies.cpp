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
#include "Pathfinding.h"
#include "Map.h"


WEnemies::WEnemies() : Entity(EntityType::WENEMIES)
{
	name.Create("wenemy");
	currentState = EnemyState::MOVING_TO_DESTINATION; // Inicialmente, el enemigo se mueve hacia la posición de destino
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


	
	enemy1WalkAnimR = GetAnimation("enemy1WalkAnimR");
	enemy1WalkAnimL = GetAnimation("enemy1WalkAnimL");
	
}

bool WEnemies::Awake()
{
	position = iPoint(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());

	return true;
}

bool WEnemies::Start() {

	WEnemiesStartAnims();

	wenemyDeathFx = app->audio->LoadFx("Assets/Audio/Fx/wenemyDeath.wav");

	enemyTex1 = app->tex->Load(parameters.attribute("texturepath").as_string());
	//crea un bool death
	if (death == false)
	{
		ebody = app->physics->CreateCircle(position.x + 16, position.y + 16, 8, bodyType::DYNAMIC);
		ebody->listener = this;
		ebody->ctype = ColliderType::WENEMIES;
	}

	currentAnimation = enemy1WalkAnimL;


	

	return true;
}

bool WEnemies::Update(float dt)
{
	if (death == false)
	{
		dt = 16;

		b2Vec2 impulse = b2Vec2_zero;
		b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);

		switch (currentState)
		{
		case EnemyState::MOVING_TO_DESTINATION:
			MoveToDestination(dt);
			impulse.x -= acceleration;
			vel = b2Vec2(speed * dt, -GRAVITY_Y);
			break;
		case EnemyState::MOVING_TO_ORIGIN:
			MoveToOrigin(dt);
			impulse.x += acceleration;
			vel = b2Vec2(-speed * dt, -GRAVITY_Y);
			break;
		case EnemyState::ATTACKING:

			//condicional para saber si va a la derecha o a la izquierda
			if (app->scene->GetPlayerPosition().x < position.x)
			{
				currentAnimation = enemy1WalkAnimL;
				currentAnimation->Update();
				impulse.x -= acceleration;
				vel = b2Vec2(speed * dt, -GRAVITY_Y);
				//si el personaje esta fuera del path
				if (app->map->WorldToMap(position.x, position.y).DistanceTo(app->map->WorldToMap(app->scene->GetPlayerPosition().x, app->scene->GetPlayerPosition().y)) > 7)
				{
					currentState = EnemyState::MOVING_TO_DESTINATION;
					app->map->pathfinding->CreatePath(app->map->WorldToMap(position.x, position.y),
						app->map->WorldToMap(app->scene->GetPlayerPosition().x, app->scene->GetPlayerPosition().y));
				}
			}
			else
			{
				currentAnimation = enemy1WalkAnimR;
				currentAnimation->Update();
				impulse.x += acceleration;
				vel = b2Vec2(-speed * dt, -GRAVITY_Y);
				//si el personaje esta fuera del path
				if (app->map->WorldToMap(position.x, position.y).DistanceTo(app->map->WorldToMap(app->scene->GetPlayerPosition().x, app->scene->GetPlayerPosition().y)) > 7)
				{
					currentState = EnemyState::MOVING_TO_ORIGIN;
					app->map->pathfinding->CreatePath(app->map->WorldToMap(position.x, position.y),
						app->map->WorldToMap(app->scene->GetPlayerPosition().x, app->scene->GetPlayerPosition().y));
				}
			}
			break;
		}

		//Set the velocity of the pbody of the player
		ebody->body->ApplyLinearImpulse(impulse, ebody->body->GetPosition(), false);
		ebody->body->SetLinearVelocity(b2Clamp(ebody->body->GetLinearVelocity(), -vel, vel));

		// Update the character's position
		b2Transform ebodyPos = ebody->body->GetTransform();
		position.x = METERS_TO_PIXELS(ebodyPos.p.x) - 16 / 2;
		position.y = METERS_TO_PIXELS(ebodyPos.p.y) - 16 / 2;

		// Pathfinding

		// pathfinding entre el enemigo y el player

		//conditional to check if player is 5 tiles to the enemy

		if (app->map->WorldToMap(position.x, position.y).DistanceTo(app->map->WorldToMap(app->scene->GetPlayerPosition().x, app->scene->GetPlayerPosition().y)) < 7)
		{
			currentState = EnemyState::ATTACKING;
			app->map->pathfinding->CreatePath(app->map->WorldToMap(position.x, position.y),
				app->map->WorldToMap(app->scene->GetPlayerPosition().x, app->scene->GetPlayerPosition().y));
		}
		// pathfinding next steps
		const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();

		//const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
		if (app->physics->debug == true)
		{
			for (uint i = 0; i < path->Count(); ++i)
			{
				iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				app->render->DrawTexture(app->scene->mouseTileTex, pos.x, pos.y);
			}
		}
		else
		{
			for (uint i = 0; i < path->Count(); ++i)
			{
				iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			}
		}

		app->render->DrawTexture(enemyTex1, position.x, position.y, &currentAnimation->GetCurrentFrame());
	}
	if (death == true)
	{
		app->physics->DestroyCircle(ebody);
		ebody = app->physics->CreateCircle(-100 + 16, 1000 + 16, 8, bodyType::DYNAMIC);
	}
	return true;
}



bool WEnemies::CleanUp()
{

	return true;
}



void WEnemies::OnCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::JABON:
		break;

	case ColliderType::PLATFORM:

		break;

	case ColliderType::DEATH:
			
		break;
	case ColliderType::UNKNOWN:
		break;
	case ColliderType::PLAYER:
		//conditional to check if player jumps on the enemy
		if (app->scene->GetPlayerPosition().y < position.y)
		{	
			death = true;	
			app->scene->GetPlayerLivesAlive();
			app->audio->PlayFx(wenemyDeathFx);
			app->scene->GetWEnemyDeathScore();
		}
		else
		{
			app->scene->GetPlayerDeath();
			death = false;
		}
		break;
	}
}

void WEnemies::MoveToDestination(float dt)
{
	// Update the character's position
	b2Transform ebodyPos = ebody->body->GetTransform();
	position.x = METERS_TO_PIXELS(ebodyPos.p.x) - 16 / 2;
	position.y = METERS_TO_PIXELS(ebodyPos.p.y) - 16 / 2;


	currentAnimation = enemy1WalkAnimL;
	currentAnimation->Update();

	if (HasReachedDestination())
	{
		currentState = EnemyState::MOVING_TO_ORIGIN;
	}
}

void WEnemies::MoveToOrigin(float dt)
{
	currentAnimation = enemy1WalkAnimR;
	currentAnimation->Update();

	if (HasReachedOrigin())
	{
		currentState = EnemyState::MOVING_TO_DESTINATION;
	}
}

bool WEnemies::HasReachedDestination()
{
	// Update the character's position
	b2Transform ebodyPos = ebody->body->GetTransform();
	position.x = METERS_TO_PIXELS(ebodyPos.p.x) - 16 / 2;
	position.y = METERS_TO_PIXELS(ebodyPos.p.y) - 16 / 2;

	if (position.x == 100 || position.x == 650 || position.x == 1872)
	{
		return true; // Cambia esto con tu lógica real
	}
	else {
		return false;
	}
}

bool WEnemies::HasReachedOrigin()
{
	// Update the character's position

	b2Transform ebodyPos = ebody->body->GetTransform();
	position.x = METERS_TO_PIXELS(ebodyPos.p.x) - 16 / 2;
	position.y = METERS_TO_PIXELS(ebodyPos.p.y) - 16 / 2;

	if (position.x == 250 || position.x == 765 || position.x == 2098)
	{
		return true; // Cambia esto con tu lógica real
	}
	else {
		return false;
	}
}


Animation* WEnemies::GetAnimation(SString name)
{
	for (ListItem<Animation*>* item = animationList.start; item != nullptr; item = item->next)
	{
		if (item->data != nullptr) {
			if (item->data->name == name) return item->data;
		}
	}
	return nullptr;
}
