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

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

void Player::PlayerStartAnims()
{
	playerR.PushBack({ 19, 33, 16, 16 });
	playerR.PushBack({ 35, 32, 16, 16 });
	playerR.PushBack({ 35, 32, 16, 16 });
	playerR.PushBack({ 35, 32, 16, 16 });
	playerR.PushBack({ 35, 32, 16, 16 });
	playerR.PushBack({ 52, 33, 16, 16 });

	idleR.PushBack({ 19, 17, 16, 16 });
	idleR.PushBack({ 35, 16, 16, 16 });
	idleR.PushBack({ 51, 17, 16, 16 });

	playerL.PushBack({ 52, 33, 16, 16 });
	playerL.PushBack({ 35, 32, 16, 16 });
	playerL.PushBack({ 35, 32, 16, 16 });
	playerL.PushBack({ 35, 32, 16, 16 });
	playerL.PushBack({ 35, 32, 16, 16 });
	playerL.PushBack({ 19, 33, 16, 16 });

	jumpR.PushBack({ 16, 48, 16, 16 });
	jumpR.PushBack({ 16, 48, 16, 16 });
	jumpR.PushBack({ 16, 48, 16, 16 });
	jumpR.PushBack({ 16, 48, 16, 16 });
	jumpR.PushBack({ 16, 48, 16, 16 });

	deathR.PushBack({81, 15, 16, 16});
}


bool Player::Awake() 
{
	position = iPoint(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());
	
	return true;
}

bool Player::Start() {

	//initilize textures

	PlayerStartAnims();
	texture = app->tex->Load(parameters.attribute("texturepath").as_string());
	
	
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 8, bodyType::DYNAMIC);

	currentAnimation = &idleR;

	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	

	return true;
}

bool Player::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN){
		godMode = !godMode;
	}
	

	if (godMode != true) {
		if (death == false)
		{
			app->scene->pause = false;
			b2Vec2 impulse = b2Vec2_zero;
			b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && jumpsAvaiable > 0) {

				currentAnimation = &jumpR;
				jumpR.Update();
				impulse.y -= jumpPower;
				jumpsAvaiable--;
			}


			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				currentAnimation = &playerL;
				playerL.Update();
				if (app->input->GetKey(SDL_SCANCODE_SPACE))
				{
					currentAnimation = &jumpR;
					jumpR.Update();

				}
				impulse.x -= acceleration;
				vel = b2Vec2(speed * dt, -GRAVITY_Y);
			}

			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				currentAnimation = &playerR;
				playerR.Update();
				if (app->input->GetKey(SDL_SCANCODE_SPACE))
				{
					playerR.HasFinished();
					currentAnimation = &jumpR;
					jumpR.Update();
				}

				impulse.x += acceleration;
				vel = b2Vec2(-speed * dt, -GRAVITY_Y);


			}


			// Hace que salte
			impulse.y = b2Clamp(impulse.y, -vel.y, vel.y);

			//Set the velocity of the pbody of the player

			pbody->body->ApplyLinearImpulse(impulse, pbody->body->GetPosition(), false);
			pbody->body->SetLinearVelocity(b2Clamp(pbody->body->GetLinearVelocity(), -vel, vel));

			//Update player position in pixels
			b2Transform pbodyPos = pbody->body->GetTransform();
			position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 16 / 2;
			position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 16 / 2;
			app->render->DrawTexture(texture, position.x, position.y, &(currentAnimation->GetCurrentFrame()));

			uint w, h;
			app->win->GetWindowSize(w, h);
			app->render->camera.x = (-position.x * app->win->GetScale()) + w / 2;

		}

		else if (death == true)
		{
			currentAnimation = &deathR;
			app->scene->pause = true;

			app->physics->DestroyCircle(pbody);

			pbody = app->physics->CreateCircle(3 + 16, 180 + 16, 8, bodyType::DYNAMIC);

			position = iPoint(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());
			app->render->DrawTexture(texture, position.x, position.y, &(currentAnimation->GetCurrentFrame()));
			texture = app->tex->Load(parameters.attribute("texturepath").as_string());

			pbody->listener = this;
			pbody->ctype = ColliderType::PLAYER;

		}
		if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		{
			app->scene->pause = true;

			app->physics->DestroyCircle(pbody);

			pbody = app->physics->CreateCircle(3 + 16, 180 + 16, 8, bodyType::DYNAMIC);

			position = iPoint(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());
			app->render->DrawTexture(texture, position.x, position.y, &(currentAnimation->GetCurrentFrame()));
			texture = app->tex->Load(parameters.attribute("texturepath").as_string());

			pbody->listener = this;
			pbody->ctype = ColliderType::PLAYER;
		}
	}
	if (godMode == true)
	{
		app->scene->pause = false;
		b2Vec2 impulse = b2Vec2_zero;
		b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && jumpsAvaiable > 0) {

			currentAnimation = &jumpR;
			jumpR.Update();
			impulse.y -= jumpPower;
			jumpsAvaiable--;
		}


		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			currentAnimation = &playerL;
			playerL.Update();
			if (app->input->GetKey(SDL_SCANCODE_SPACE))
			{
				currentAnimation = &jumpR;
				jumpR.Update();

			}
			impulse.x -= acceleration;
			vel = b2Vec2(speed * dt, -GRAVITY_Y);
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			currentAnimation = &playerR;
			playerR.Update();
			if (app->input->GetKey(SDL_SCANCODE_SPACE))
			{
				playerR.HasFinished();
				currentAnimation = &jumpR;
				jumpR.Update();
			}

			impulse.x += acceleration;
			vel = b2Vec2(-speed * dt, -GRAVITY_Y);


		}


		// Hace que salte
		impulse.y = b2Clamp(impulse.y, -vel.y, vel.y);

		//Set the velocity of the pbody of the player

		pbody->body->ApplyLinearImpulse(impulse, pbody->body->GetPosition(), false);
		pbody->body->SetLinearVelocity(b2Clamp(pbody->body->GetLinearVelocity(), -vel, vel));

		//Update player position in pixels
		b2Transform pbodyPos = pbody->body->GetTransform();
		position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 16 / 2;
		position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 16 / 2;
		app->render->DrawTexture(texture, position.x, position.y, &(currentAnimation->GetCurrentFrame()));

		uint w, h;
		app->win->GetWindowSize(w, h);
		app->render->camera.x = (-position.x * app->win->GetScale()) + w / 2;

	}
	
	

	return true;
}



bool Player::CleanUp()
{

	return true;
}



void Player::OnCollision(PhysBody* physA, PhysBody* physB) 
{
	switch (physB->ctype)
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
	}
}