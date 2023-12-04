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

	enemyTex1 = app->tex->Load(parameters.attribute("texturepath").as_string());

	ebody = app->physics->CreateCircle(position.x + 16, position.y + 16, 8, bodyType::DYNAMIC);

	currentAnimation = enemy1WalkAnimL;

	ebody->listener = this;
	ebody->ctype = ColliderType::WENEMIES;

	return true;
}

bool WEnemies::Update(float dt)
{
	b2Vec2 impulse = b2Vec2_zero;
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
	
	currentAnimation = enemy1WalkAnimL;
	currentAnimation->Update();

	impulse.x -= acceleration;
	vel = b2Vec2(speed * dt, -GRAVITY_Y);
	
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



bool WEnemies::CleanUp()
{

	return true;
}



void WEnemies::OnCollision(PhysBody* physA, PhysBody* physB)
{
	b2Vec2 impulse = b2Vec2_zero;
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);

	float dt = 16;

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		break;

	case ColliderType::PLATFORM:

		break;
	case ColliderType::DEATH:
		break;

	case ColliderType::WALLENEMYL:

		currentAnimation = enemy1WalkAnimR;
		currentAnimation->Update();

		impulse.x += acceleration;
		vel = b2Vec2(-speed * dt, -GRAVITY_Y);

		break;

	case ColliderType::WALLENEMYR:

		currentAnimation = enemy1WalkAnimL;
		currentAnimation->Update();

		impulse.x -= acceleration;
		vel = b2Vec2(speed * dt, -GRAVITY_Y);
		break;

	case ColliderType::UNKNOWN:
		break;
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
