#pragma once

#include "Boss.h"
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
#include "FadeToBlack.h"


Boss::Boss() : Entity(EntityType::BOSS)
{
	name.Create("Boss");
	currentState = EnemyBossState::MOVING_TO_DESTINATION; // Inicialmente, el enemigo se mueve hacia la posición de destino
}

Boss::~Boss() {

}

void Boss::BossStartAnims()
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

	bossWalkL = GetAnimation("bossWalkL");
	bossWalkR = GetAnimation("bossWalkR");

}

bool Boss::Awake()
{
	position = iPoint(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());

	return true;
}

bool Boss::Start() {

	BossStartAnims();

	bossDeathFx = app->audio->LoadFx("Assets/Music/levelCompleted.wav");

	enemyTex1 = app->tex->Load(parameters.attribute("texturepath").as_string());

	bossbody = app->physics->CreateCircle(position.x + 33, position.y + 50, 24, bodyType::DYNAMIC);

	currentAnimation = bossWalkL;

	bossbody->listener = this;
	bossbody->ctype = ColliderType::BOSS;

	boss5lives = app->tex->Load("Assets/Textures/boss5vida.png");
	boss4lives = app->tex->Load("Assets/Textures/boss4vida.png");
	boss3lives = app->tex->Load("Assets/Textures/boss3vida.png");
	boss2lives = app->tex->Load("Assets/Textures/boss2vida.png");
	boss1lives = app->tex->Load("Assets/Textures/boss1vida.png");

	return true;
}

bool Boss::Update(float dt)
{
	if (lives == 5)
	{
		app->render->DrawTexture(boss5lives, app->scene->GetPlayerPosition().x + 40, 1623);
	}
	if (lives == 4)
	{
		app->render->DrawTexture(boss4lives, app->scene->GetPlayerPosition().x + 40, 1623);
	}
	if (lives == 3)
	{
		app->render->DrawTexture(boss3lives, app->scene->GetPlayerPosition().x + 40, 1623);
	}
	if (lives == 2)
	{
		app->render->DrawTexture(boss2lives, app->scene->GetPlayerPosition().x + 40, 1623);
	}
	if (lives == 1)
	{
		app->render->DrawTexture(boss1lives, app->scene->GetPlayerPosition().x + 40, 1623);
	}
	if (app->scene->pause == false)
	{
		if (death == false)
		{
			dt = 16;

			b2Vec2 impulse = b2Vec2_zero;
			b2Vec2 vel = b2Vec2(0, 0);

			bossbody->body->SetGravityScale(0);

			switch (currentState)
			{
			case EnemyBossState::MOVING_TO_DESTINATION:
				MoveToDestination(dt);
				impulse.x -= acceleration;
				vel = b2Vec2(speed * dt, 0);
				break;
			case EnemyBossState::MOVING_TO_ORIGIN:
				MoveToOrigin(dt);
				impulse.x += acceleration;
				vel = b2Vec2(-speed * dt, 0);
				break;
			case EnemyBossState::ATTACKING:

				//condicional para saber si va a la derecha o a la izquierda
				if (app->scene->GetPlayerPosition().x < position.x)
				{
					currentAnimation = bossWalkL;
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
						currentState = EnemyBossState::MOVING_TO_DESTINATION;
						app->map->pathfinding->CreatePath(app->map->WorldToMap(position.x, position.y),
							app->map->WorldToMap(app->scene->GetPlayerPosition().x, app->scene->GetPlayerPosition().y));
					}
				}
				else
				{
					currentAnimation = bossWalkR;
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
						currentState = EnemyBossState::MOVING_TO_ORIGIN;
						app->map->pathfinding->CreatePath(app->map->WorldToMap(position.x, position.y),
							app->map->WorldToMap(app->scene->GetPlayerPosition().x, app->scene->GetPlayerPosition().y));
					}
				}
				break;
			}


			//Set the velocity of the pbody of the player
			bossbody->body->ApplyLinearImpulse(impulse, bossbody->body->GetPosition(), false);
			bossbody->body->SetLinearVelocity(b2Clamp(bossbody->body->GetLinearVelocity(), -vel, vel));

			// Update the character's position
			b2Transform bossbodyPos = bossbody->body->GetTransform();
			position.x = METERS_TO_PIXELS(bossbodyPos.p.x) - 16 / 2;
			position.y = METERS_TO_PIXELS(bossbodyPos.p.y) - 16 / 2;

			// Pathfinding

			// pathfinding entre el enemigo y el player

			//conditional to check if player is 7 tiles to the enemy

			if (app->map->WorldToMap(position.x, position.y).DistanceTo(app->map->WorldToMap(app->scene->GetPlayerPosition().x, app->scene->GetPlayerPosition().y)) < 7)
			{
				currentState = EnemyBossState::ATTACKING;
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
		if (lives == 0)
		{
			app->physics->DestroyCircle(bossbody);
			bossbody = app->physics->CreateCircle(-100 + 16, 1000 + 16, 8, bodyType::DYNAMIC);
			app->scene->WinScore();
			
			//go to end screen
			app->audio->PlayFx(bossDeathFx);
		
		}
		if (position.y < 1820)
		{
			app->physics->DestroyCircle(bossbody);
			bossbody = app->physics->CreateCircle(500 + 33, 1875 + 50, 24, bodyType::DYNAMIC);
		}
	}
	return true;
}



bool Boss::CleanUp()
{

	return true;
}



void Boss::OnCollision(PhysBody* physA, PhysBody* physB)
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
		break;

	case ColliderType::PLAYER_PROYECTILE:
		lives = lives - 1;

		break;
	}
}

void Boss::MoveToDestination(float dt)
{
	// Update the character's position
	b2Transform bossbodyPos = bossbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(bossbodyPos.p.x) - 16 / 2;
	position.y = METERS_TO_PIXELS(bossbodyPos.p.y) - 16 / 2;


	currentAnimation = bossWalkL;
	currentAnimation->Update();

	if (HasReachedDestination())
	{
		currentState = EnemyBossState::MOVING_TO_ORIGIN;
	}
}

void Boss::MoveToOrigin(float dt)
{
	currentAnimation = bossWalkR;
	currentAnimation->Update();

	if (HasReachedOrigin())
	{
		currentState = EnemyBossState::MOVING_TO_DESTINATION;
	}
}

bool Boss::HasReachedDestination()
{
	// Update the character's position
	b2Transform bossbodyPos = bossbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(bossbodyPos.p.x) - 16 / 2;
	position.y = METERS_TO_PIXELS(bossbodyPos.p.y) - 16 / 2;

	if (position.x == 200 || position.x == 994 || position.x == 2553 || position.x == 2107 || position.x == 3112 || position.x == 3650 || position.x == 3810)
	{
		return true; // Cambia esto con tu lógica real
	}
	else {
		return false;
	}
}

bool Boss::HasReachedOrigin()
{
	// Update the character's position
	b2Transform bossbodyPos = bossbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(bossbodyPos.p.x) - 16 / 2;
	position.y = METERS_TO_PIXELS(bossbodyPos.p.y) - 16 / 2;

	if (position.x == 500 || position.x == 1382 || position.x == 3030 || position.x == 2358 || position.x == 3479 || position.x == 3774 || position.x == 3981)
	{
		return true; // Cambia esto con tu lógica real
	}
	else {
		return false;
	}
}


Animation* Boss::GetAnimation(SString name)
{
	for (ListItem<Animation*>* item = animationList.start; item != nullptr; item = item->next)
	{
		if (item->data != nullptr) {
			if (item->data->name == name) return item->data;
		}
	}
	return nullptr;
}
