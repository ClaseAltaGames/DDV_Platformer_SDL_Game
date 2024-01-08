#pragma once

#include "Health.h"
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


Poti::Poti() : Entity(EntityType::HEALTH)
{
	name.Create("poti");
}

Poti::~Poti() {

}


bool Poti::Awake()
{
	position = iPoint(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());

	return true;
}

bool Poti::Start() {

	potiTex = app->tex->Load(parameters.attribute("texturepath").as_string());

	potibody = app->physics->CreateCircle(position.x + 16, position.y + 16, 8, bodyType::DYNAMIC);
	potibody->listener = this;
	potibody->ctype = ColliderType::HEALTH;

	return true;
}

bool Poti::Update(float dt)
{
	b2Transform potiBodyPos = potibody->body->GetTransform();
	position.x = METERS_TO_PIXELS(potiBodyPos.p.x) - 16 / 2;
	position.y = METERS_TO_PIXELS(potiBodyPos.p.y) - 16 / 2;

	app->render->DrawTexture(potiTex, position.x, position.y);
	
	if (isPicked == true)
	{
		app->physics->DestroyCircle(potibody);
		potibody = app->physics->CreateCircle(-1000 + 16, 1000 + 16, 8, bodyType::DYNAMIC);
				
	}
		

	return true;
}



bool Poti::CleanUp()
{

	return true;
}



void Poti::OnCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		isPicked = true;


		break;
	}
}


