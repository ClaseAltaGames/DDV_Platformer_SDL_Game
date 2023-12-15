#pragma once

#include "WEnemies2.h"
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


WEnemies2::WEnemies2() : Entity(EntityType::WENEMIES)
{
	name.Create("wenemy");
	currentState = EnemyState::MOVING_TO_DESTINATION; // Inicialmente, el enemigo se mueve hacia la posición de destino
}

WEnemies2::~WEnemies2() {

}

void WEnemies2::WEnemiesStartAnims()
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

bool WEnemies2::Awake()
{
	position = iPoint(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());

	return true;
}

bool WEnemies2::Start() {

	WEnemiesStartAnims();


	enemyTex1 = app->tex->Load(parameters.attribute("texturepath").as_string());

	ebody = app->physics->CreateCircle(position.x + 16, position.y + 16, 8, bodyType::DYNAMIC);

	currentAnimation = enemy1WalkAnimL;

	ebody->listener = this;
	ebody->ctype = ColliderType::WENEMIES;



	return true;
}

bool WEnemies2::Update(float dt)
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
	}





	//Set the velocity of the pbody of the player
	ebody->body->ApplyLinearImpulse(impulse, ebody->body->GetPosition(), false);
	ebody->body->SetLinearVelocity(b2Clamp(ebody->body->GetLinearVelocity(), -vel, vel));

	// Update the character's position
	b2Transform ebodyPos = ebody->body->GetTransform();
	position.x = METERS_TO_PIXELS(ebodyPos.p.x) - 16 / 2;
	position.y = METERS_TO_PIXELS(ebodyPos.p.y) - 16 / 2;

	// Pathfinding
	//app->map->pathfinding->CreatePath(position, player->position);

	// pathginding entre el enemigo y el player
	app->map->pathfinding->CreatePath(position, player->position);

	// pathfinding next steps
	const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();

	//// condicional: si el jugador esta a menos de 100 pixeles de distancia del enemigo, el enemigo comienza a hacer pathfinding
	//if (position.DistanceTo(player->position) < 100)
	//{
	//	// condicional: si el pathfinding tiene mas de 0 pasos, el enemigo se mueve hacia el jugador
	//	if (path->Count() > 0)
	//	{
	//		// actualiza la posicion del enemigo
	//		
	//		//iPoint nextStep = (*path)[1];
	//		//iPoint direction = nextStep - position;
	//		
	//		// posicion con el impulse
	//		impulse.x += acceleration;

	//		
	//	}
	//}

	app->render->DrawTexture(enemyTex1, position.x, position.y, &currentAnimation->GetCurrentFrame());

	return true;
}



bool WEnemies2::CleanUp()
{

	return true;
}



void WEnemies2::OnCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		break;

	case ColliderType::PLATFORM:

		break;

	case ColliderType::DEATH:
		//si el player esta encima del enemigo, el enemigo muere

		app->physics->DestroyCircle(ebody);
		//delete enemy texture
		app->tex->UnLoad(enemyTex1);
		break;
	case ColliderType::UNKNOWN:
		break;
	case ColliderType::PLAYER:
		/*if (player == nullptr && ebody->body->GetPosition().y > player->pbody->body->GetPosition().y)*/

		app->physics->DestroyCircle(ebody);
		//delete enemy texture
		app->tex->UnLoad(enemyTex1);

		break;
	}
}

void WEnemies2::MoveToDestination(float dt)
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

void WEnemies2::MoveToOrigin(float dt)
{
	currentAnimation = enemy1WalkAnimR;
	currentAnimation->Update();

	if (HasReachedOrigin())
	{
		currentState = EnemyState::MOVING_TO_DESTINATION;
	}
}

bool WEnemies2::HasReachedDestination()
{
	// Update the character's position
	b2Transform ebodyPos = ebody->body->GetTransform();
	position.x = METERS_TO_PIXELS(ebodyPos.p.x) - 16 / 2;
	position.y = METERS_TO_PIXELS(ebodyPos.p.y) - 16 / 2;

	if (position.x == 100)
	{
		return true; // Cambia esto con tu lógica real
	}
	else {
		return false;
	}
}

bool WEnemies2::HasReachedOrigin()
{
	// Update the character's position
	b2Transform ebodyPos = ebody->body->GetTransform();
	position.x = METERS_TO_PIXELS(ebodyPos.p.x) - 16 / 2;
	position.y = METERS_TO_PIXELS(ebodyPos.p.y) - 16 / 2;

	if (position.x == 250)
	{
		return true; // Cambia esto con tu lógica real
	}
	else {
		return false;
	}
}


Animation* WEnemies2::GetAnimation(SString name)
{
	for (ListItem<Animation*>* item = animationList.start; item != nullptr; item = item->next)
	{
		if (item->data != nullptr) {
			if (item->data->name == name) return item->data;
		}
	}
	return nullptr;
}
