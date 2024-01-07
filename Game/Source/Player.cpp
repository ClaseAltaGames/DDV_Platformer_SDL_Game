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
#include "WEnemies.h"
#include "FEnemies.h"
#include "LevelCompletedScreen.h"
#include "FadeToBlack.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "DeadScreen.h"


Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

void Player::PlayerStartAnims()
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

	idleR = GetAnimation("idleR");
	playerR = GetAnimation("playerR");
	playerL = GetAnimation("playerL");
	jumpR = GetAnimation("jumpR");
	deathR = GetAnimation("deathR");
	
}


bool Player::Awake() 
{
	position = iPoint(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());
	
	return true;
}

bool Player::Start() {

	//initilize textures

	oneLive = app->tex->Load("Assets/Textures/1vidas.png");
	twoLive = app->tex->Load("Assets/Textures/2vidas.png");
	threeLive = app->tex->Load("Assets/Textures/3vidas.png");

	texture = app->tex->Load(parameters.attribute("texturepath").as_string());
	
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 8, bodyType::DYNAMIC);

	currentAnimation = idleR;

	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	gravityScale = pbody->body->GetGravityScale();

	saltoFX = app->audio->LoadFx("Assets/Audio/Fx/saltoFX.wav");
	playerDeathFx = app->audio->LoadFx("Assets/Music/wasted.wav");
	pickShampooFx = app->audio->LoadFx("Assets/Music/pickShampoo.wav");

	PlayerStartAnims();
	

	return true;
}

bool Player::Update(float dt)
{
	points.Create("POINTS: %d", score);

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN){
		godMode = !godMode;
	}

	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {
		if (app->maxFrameDuration == 16) {
			app->maxFrameDuration = 32;
		}
		else if(app->maxFrameDuration == 32) {
			app->maxFrameDuration = 16;
		}
	}
	

	if (godMode == false) {
		if (death == false)
		{
			if (position.y < 1000)
			{
				app->scene->level1 = true;
				app->scene->level2 = false;
			}
			if (position.y > 1000)
			{
				app->scene->level2 = true;
				app->scene->level1 = false;
			}
			if (lives == 3)
			{
				app->render->DrawTexture(threeLive, position.x - 230, 20);
				app->render->DrawTexture(threeLive, position.x - 230, 885);
			}
			if (lives == 2)
			{
				app->render->DrawTexture(twoLive, position.x - 230, 20);
				app->render->DrawTexture(twoLive, position.x - 230, 885);
			}
			if (lives == 1)
			{
				app->render->DrawTexture(oneLive, position.x - 230, 20);
				app->render->DrawTexture(oneLive, position.x - 230, 885);
			}

			app->scene->pause = false;
			speed = 0.12f;

			pbody->body->SetGravityScale(gravityScale);

			b2Vec2 impulse = b2Vec2_zero;
			b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && jumpsAvaiable > 0) {
				app->audio->PlayFx(saltoFX);
				currentAnimation = jumpR;
				currentAnimation->Update();
				impulse.y -= jumpPower;
				jumpsAvaiable--;
			}


			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				currentAnimation = playerL;
				currentAnimation->Update();
				if (app->input->GetKey(SDL_SCANCODE_SPACE))
				{
					currentAnimation = jumpR;
					currentAnimation->Update();
				}
				impulse.x -= acceleration;
				vel = b2Vec2(speed * dt, -GRAVITY_Y);
			}

			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				currentAnimation = playerR;
				currentAnimation->Update();
				if (app->input->GetKey(SDL_SCANCODE_SPACE))
				{
					currentAnimation = jumpR;
					currentAnimation->Update();
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

		if (death == true && lives == 0)
		{
			//fade to black to death screen
			app->fadeToBlack->FadeToBlackTransition((Module*)app->scene, (Module*)app->deadScreen, 0);
			app->deadScreen->active = true;
			app->deadScreen->Enable();

			//unload the scene music
			app->audio->UnloadMusic();

			app->audio->PlayFx(playerDeathFx);

			lives = 3;

			currentAnimation = deathR;
			currentAnimation->Update();

			app->scene->pause = true;

			app->physics->DestroyCircle(pbody);
			
			if (position.y < 1000)
			{
				pbody = app->physics->CreateCircle(3 + 16, 180 + 16, 8, bodyType::DYNAMIC);
			}
			if (position.y > 1000)
			{
				pbody = app->physics->CreateCircle(39 + 16, 1119 + 16, 8, bodyType::DYNAMIC);
				app->scene->CameraLevel2();
			}
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

			if (position.y < 1000)
			{
				pbody = app->physics->CreateCircle(3 + 16, 180 + 16, 8, bodyType::DYNAMIC);
				app->scene->CameraLevel1();
			}
			if (position.y > 1000)
			{
				pbody = app->physics->CreateCircle(39 + 16, 1119 + 16, 8, bodyType::DYNAMIC);
				app->scene->CameraLevel2();
			}
			position = iPoint(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());
			app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame());
			texture = app->tex->Load(parameters.attribute("texturepath").as_string());

			pbody->listener = this;
			pbody->ctype = ColliderType::PLAYER;
		}
		if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		{
			app->scene->pause = true;

			app->physics->DestroyCircle(pbody);

			pbody = app->physics->CreateCircle(39 + 16, 1119 + 16, 8, bodyType::DYNAMIC);
			app->scene->CameraLevel2();
			
			position = iPoint(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());
			app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame());
			texture = app->tex->Load(parameters.attribute("texturepath").as_string());

			pbody->listener = this;
			pbody->ctype = ColliderType::PLAYER;
		}
		if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		{
			app->scene->pause = true;

			app->physics->DestroyCircle(pbody);
			pbody = app->physics->CreateCircle(3 + 16, 180 + 16, 8, bodyType::DYNAMIC);

			app->scene->CameraLevel1();
			
			position = iPoint(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());
			app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame());
			texture = app->tex->Load(parameters.attribute("texturepath").as_string());

			pbody->listener = this;
			pbody->ctype = ColliderType::PLAYER;
		}
	}
	if (godMode == true) {
		app->scene->pause = false;
		speed = 0.35f;
		b2Vec2 impulse = b2Vec2_zero;
		b2Vec2 vel = b2Vec2(0, 0);

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			currentAnimation = idleR;
			impulse.x -= acceleration;
			vel = b2Vec2(speed * dt, vel.y);
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			currentAnimation = idleR;
			impulse.x += acceleration;
			vel = b2Vec2(-speed * dt, vel.y);
		}

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			currentAnimation = idleR;
			impulse.y -= acceleration;
			vel = b2Vec2(vel.x, speed * dt);
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			currentAnimation = idleR;
			impulse.y += acceleration;
			vel = b2Vec2(vel.x, -speed * dt);
		}
		
		pbody->body->SetGravityScale(0);
		// Apply the impulse to the character's rigidbody
		pbody->body->ApplyLinearImpulse(impulse, pbody->body->GetPosition(), false);

		// Clamp the character's velocity to prevent excessive speed
		pbody->body->SetLinearVelocity(b2Clamp(pbody->body->GetLinearVelocity(), -vel, vel));

		// Update the character's position
		b2Transform pbodyPos = pbody->body->GetTransform();
		position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 16 / 2;
		position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 16 / 2;

		// Draw the character's texture
		app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame());

		// Update the camera position
		uint w, h;
		app->win->GetWindowSize(w, h);
		app->render->camera.x = (-position.x * app->win->GetScale()) + w / 2;


	}

	app->render->DrawText(points.GetString(), 900, 30, 100, 50);

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
	case ColliderType::JABON:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickShampooFx);
		score += 50;
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		currentAnimation = idleR;
		jumpsAvaiable = 1;
		app->scene->pause = false;
		death = false;
		break;
	case ColliderType::DEATH:
		LOG("Collision DEATH");
		death = true;
		lives--;
		break;
	case ColliderType::COMPLETED:
		LOG("Collision COMPLETED");
		app->audio->UnloadMusic();

		app->levelCompletedScreen->active = true;
		app->levelCompletedScreen->Enable();

		app->fadeToBlack->FadeToBlackTransition((Module*)app->scene, (Module*)app->levelCompletedScreen, 0);
		app->audio->PlayFx(app->levelCompletedScreen->completedSound);		
		
		break;
	case ColliderType::WENEMIES:
		LOG("Collision WENEMIES");
		death = true;
		break;
	case ColliderType::FENEMIES:
		LOG("Collision FENEMIES");
		death = true;
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}

}

Animation* Player::GetAnimation(SString name)
{
	for (ListItem<Animation*>* item = animationList.start; item != nullptr; item = item->next)
	{
		if (item->data != nullptr) {
			if (item->data->name == name) return item->data;
		}
	}
	return nullptr;
}

