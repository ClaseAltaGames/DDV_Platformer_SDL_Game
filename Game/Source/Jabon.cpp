#pragma once

#include "Jabon.h"
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


Jabon::Jabon() : Entity(EntityType::JABON)
{
	name.Create("jabon");
}

Jabon::~Jabon() {

}


bool Jabon::Awake()
{
	position = iPoint(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());

	return true;
}

bool Jabon::Start() {

	jabonTex = app->tex->Load(parameters.attribute("texturepath").as_string());

	jbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 8, bodyType::DYNAMIC);
	jbody->listener = this;
	jbody->ctype = ColliderType::JABON;

	return true;
}

bool Jabon::Update(float dt)
{
	b2Transform ebodyPos = jbody->body->GetTransform();
	position.x = METERS_TO_PIXELS(ebodyPos.p.x) - 16 / 2;
	position.y = METERS_TO_PIXELS(ebodyPos.p.y) - 16 / 2;

	app->render->DrawTexture(jabonTex, position.x, position.y);
	
	if (isPicked == true)
	{
		app->physics->DestroyCircle(jbody);
		jbody = app->physics->CreateCircle(-100 + 16, 1000 + 16, 8, bodyType::DYNAMIC);
	}

	return true;
}



bool Jabon::CleanUp()
{

	return true;
}



void Jabon::OnCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		isPicked = true;
		break;
	}
}


