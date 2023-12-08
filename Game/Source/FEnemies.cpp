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

	//WEnemies* enemy2 = new WEnemies();

	enemyTex1 = app->tex->Load(parameters.attribute("texturepath").as_string());

	ebody = app->physics->CreateCircle(position.x + 16, position.y + 16, 8, bodyType::DYNAMIC);

	currentAnimation = enemy1FlyAnimL;

	ebody->listener = this;
	ebody->ctype = ColliderType::FENEMIES;

	return true;
}

bool FEnemies::Update(float dt)
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
	}

	//Set the velocity of the pbody of the player
	ebody->body->ApplyLinearImpulse(impulse, ebody->body->GetPosition(), false);
	ebody->body->SetLinearVelocity(b2Clamp(ebody->body->GetLinearVelocity(), -vel, vel));

	// Update the character's position
	b2Transform ebodyPos = ebody->body->GetTransform();
	position.x = METERS_TO_PIXELS(ebodyPos.p.x) - 16 / 2;
	position.y = METERS_TO_PIXELS(ebodyPos.p.y) - 16 / 2;

	app->render->DrawTexture(enemyTex1, position.x, position.y, &currentAnimation->GetCurrentFrame());

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
	case ColliderType::ITEM:
		break;

	case ColliderType::PLATFORM:

		break;

	case ColliderType::DEATH:

		break;
	case ColliderType::UNKNOWN:

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

	if (position.x == 200)
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

	if (position.x == 500)
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
