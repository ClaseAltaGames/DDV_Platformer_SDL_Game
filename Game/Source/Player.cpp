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
	//b2Vec2 vel = b2Vec2(pbody->body->GetLocalPoint().x, -GRAVITY_Y);
	b2Vec2 impulse = b2Vec2_zero;
	float horizontalImpulse = 0.07f * speed * dt;
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);
	app->render->camera.x = -position.x;

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN /*&& jumpsAvaiable > 0*/) {
		//
		impulse.y -= jumpPower;
		//jumpsAvaiable--;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		//impulse.x -= acceleration;
		vel = b2Vec2(speed * dt, -GRAVITY_Y);
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		//impulse.x += acceleration;
		vel = b2Vec2(-speed * dt, -GRAVITY_Y);
	}
	
	//if (/*canJump == true &&*/ app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	//{
	//	float jumpImpulse = -110000; // Ajustar este valor para hacer el salto m�s suave

	//pbody->body->ApplyLinearImpulse(b2Vec2(0, impulse.y), pbody->body->GetWorldCenter(), true);

	//	if (dt < 20) {
	//		pbody->body->ApplyForceToCenter(b2Vec2(vel.x * dt, jumpImpulse), 1);
	//	}
	//	else {
	//		pbody->body->ApplyForceToCenter(b2Vec2(vel.x * dt, jumpImpulse), 1);
	//	}

	//}d
	impulse.x = b2Clamp(impulse.x, -vel.x, vel.x);

	impulse.y = b2Clamp(impulse.y, -vel.y, vel.y);
	

	//Set the velocity of the pbody of the player
	//pbody->body->SetLinearVelocity(vel);
	pbody->body->ApplyLinearImpulse(impulse, pbody->body->GetPosition(), false);
	pbody->body->SetLinearVelocity(b2Clamp(pbody->body->GetLinearVelocity(), -vel, vel));

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