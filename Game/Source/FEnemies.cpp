#pragma once

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
#include "Pathfinding.h"
#include "Map.h"


FEnemies::FEnemies() : Entity(EntityType::FENEMIES)
{
	name.Create("FEnemies");
	currentState = EnemyFlyState::MOVING_TO_DESTINATION; // Inicialmente, el enemigo se mueve hacia la posición de destino
}

FEnemies::~FEnemies() {

}

void FEnemies::FEnemiesStartAnims()
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


	enemy1FlyAnimR = GetAnimation("enemy1FlyAnimR");
	enemy1FlyAnimL = GetAnimation("enemy1FlyAnimL");

}

bool FEnemies::Awake()
{
	position = iPoint(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());

	return true;
}

bool FEnemies::Start() {

	FEnemiesStartAnims();

	fenemyDeathFx = app->audio->LoadFx("Assets/Audio/Fx/fenemyDeath.wav");

	enemyTex1 = app->tex->Load(parameters.attribute("texturepath").as_string());

	ebody = app->physics->CreateCircle(position.x + 16, position.y + 16, 8, bodyType::DYNAMIC);

	currentAnimation = enemy1FlyAnimL;

	ebody->listener = this;
	ebody->ctype = ColliderType::FENEMIES;

	return true;
}

bool FEnemies::Update(float dt)
{
	if (app->scene->pause == false)
	{
		if (death == false)
		{
			dt = 16;

			b2Vec2 impulse = b2Vec2_zero;
			b2Vec2 vel = b2Vec2(0, 0);

			ebody->body->SetGravityScale(0);

			switch (currentState)
			{
			case EnemyFlyState::MOVING_TO_DESTINATION:
				MoveToDestination(dt);
				impulse.x -= acceleration;
				vel = b2Vec2(speed * dt, 0);
				break;
			case EnemyFlyState::MOVING_TO_ORIGIN:
				MoveToOrigin(dt);
				impulse.x += acceleration;
				vel = b2Vec2(-speed * dt, 0);
				break;
			case EnemyFlyState::ATTACKING:

				//condicional para saber si va a la derecha o a la izquierda
				if (app->scene->GetPlayerPosition().x < position.x)
				{
					currentAnimation = enemy1FlyAnimL;
					currentAnimation->Update();

					impulse.x -= acceleration;
					if (app->scene->GetPlayerPosition().y < position.y)
					{
						impulse.y -= 0.001f;
					}
					else
					{
						impulse.y += 0.001f;
					}
					vel = b2Vec2(speed * dt, -GRAVITY_Y);
					//si el personaje esta fuera del path		

					if (app->map->WorldToMap(position.x, position.y).DistanceTo(app->map->WorldToMap(app->scene->GetPlayerPosition().x, app->scene->GetPlayerPosition().y)) > 7)
					{
						currentState = EnemyFlyState::MOVING_TO_DESTINATION;
						app->map->pathfinding->CreatePath(app->map->WorldToMap(position.x, position.y),
							app->map->WorldToMap(app->scene->GetPlayerPosition().x, app->scene->GetPlayerPosition().y));
					}
				}
				else
				{
					currentAnimation = enemy1FlyAnimR;
					currentAnimation->Update();
					impulse.x += acceleration;
					if (app->scene->GetPlayerPosition().y < position.y)
					{
						impulse.y -= 0.001f;
					}
					else
					{
						impulse.y += 0.001f;
					}
					vel = b2Vec2(-speed * dt, -GRAVITY_Y);
					//si el personaje esta fuera del path
					if (app->map->WorldToMap(position.x, position.y).DistanceTo(app->map->WorldToMap(app->scene->GetPlayerPosition().x, app->scene->GetPlayerPosition().y)) > 7)
					{
						currentState = EnemyFlyState::MOVING_TO_ORIGIN;
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

			//conditional to check if player is 7 tiles to the enemy

			if (app->map->WorldToMap(position.x, position.y).DistanceTo(app->map->WorldToMap(app->scene->GetPlayerPosition().x, app->scene->GetPlayerPosition().y)) < 7)
			{
				currentState = EnemyFlyState::ATTACKING;
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
	}
	return true;
}



bool FEnemies::CleanUp()
{

	return true;
}



void FEnemies::OnCollision(PhysBody* physA, PhysBody* physB)
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
		if (app->scene->GetPlayerPosition().y < position.y)
		{
			death = true;
			app->scene->GetPlayerLivesAlive();
			app->audio->PlayFx(fenemyDeathFx); 
			app->scene->GetFEnemyDeathScore();
		}
		else
		{
			app->scene->GetPlayerDeath();
			death = false;
		}
		break;
	}
}

void FEnemies::MoveToDestination(float dt)
{
	// Update the character's position
	b2Transform ebodyPos = ebody->body->GetTransform();
	position.x = METERS_TO_PIXELS(ebodyPos.p.x) - 16 / 2;
	position.y = METERS_TO_PIXELS(ebodyPos.p.y) - 16 / 2;


	currentAnimation = enemy1FlyAnimL;
	currentAnimation->Update();

	if (HasReachedDestination())
	{
		currentState = EnemyFlyState::MOVING_TO_ORIGIN;
	}
}

void FEnemies::MoveToOrigin(float dt)
{
	currentAnimation = enemy1FlyAnimR;
	currentAnimation->Update();

	if (HasReachedOrigin())
	{
		currentState = EnemyFlyState::MOVING_TO_DESTINATION;
	}
}

bool FEnemies::HasReachedDestination()
{
	// Update the character's position
	b2Transform ebodyPos = ebody->body->GetTransform();
	position.x = METERS_TO_PIXELS(ebodyPos.p.x) - 16 / 2;
	position.y = METERS_TO_PIXELS(ebodyPos.p.y) - 16 / 2;

	if (position.x == 200 || position.x == 994 || position.x == 2553 || position.x == 2107 || position.x == 3112 || position.x == 3650 || position.x == 3810)
	{
		return true; // Cambia esto con tu lógica real
	}
	else {
		return false;
	}
}

bool FEnemies::HasReachedOrigin()
{
	// Update the character's position
	b2Transform ebodyPos = ebody->body->GetTransform();
	position.x = METERS_TO_PIXELS(ebodyPos.p.x) - 16 / 2;
	position.y = METERS_TO_PIXELS(ebodyPos.p.y) - 16 / 2;

	if (position.x == 500 || position.x == 1382 || position.x == 3030 || position.x == 2358 || position.x == 3479 || position.x == 3774 || position.x == 3981)
	{
		return true; // Cambia esto con tu lógica real
	}
	else {
		return false;
	}
}


Animation* FEnemies::GetAnimation(SString name)
{
	for (ListItem<Animation*>* item = animationList.start; item != nullptr; item = item->next)
	{
		if (item->data != nullptr) {
			if (item->data->name == name) return item->data;
		}
	}
	return nullptr;
}
