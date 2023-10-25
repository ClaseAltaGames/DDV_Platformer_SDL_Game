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

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	return true;
}

bool Player::Update(float dt)
{
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		//
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		//
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		vel = b2Vec2(-speed*dt, -GRAVITY_Y);
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		vel = b2Vec2(speed*dt, -GRAVITY_Y);
	}
	
	if (/*canJump == true &&*/ app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		if (dt < 20) {
			pbody->body->ApplyForceToCenter(b2Vec2(0, -10500), 1);
		}
		else {
			pbody->body->ApplyForceToCenter(b2Vec2(0, -10500), 1);
		}

		/*if (Player_Dir == true) {
			if (currentAnimation != &jumpR)
			{
				jumpR.Reset();
				currentAnimation = &jumpR;
			}
		}
		else {
			if (currentAnimation != &jumpL)
			{
				jumpL.Reset();
				currentAnimation = &jumpL;
			}
		}*/
	}
	else {
		// Allow move when jumping / falling
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT /*&& Player_Dir == true*/) {
			pbody->body->ApplyLinearImpulse(b2Vec2(-speed * 0.07f * dt, 0), b2Vec2(0, 0), 1);
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT /*&& Player_Dir == false*/) {
			pbody->body->ApplyLinearImpulse(b2Vec2(speed * 0.07f * dt, 0), b2Vec2(0, 0), 1);
		}
	}

	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Player::CleanUp()
{

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}