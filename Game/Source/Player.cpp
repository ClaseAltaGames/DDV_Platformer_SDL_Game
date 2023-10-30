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
	playerR.PushBack({ 19, 33, 10, 16 });
	playerR.PushBack({ 35, 32, 10, 16 });
	playerR.PushBack({ 35, 32, 10, 16 });
	playerR.PushBack({ 35, 32, 10, 16 });
	playerR.PushBack({ 35, 32, 10, 16 });
	playerR.PushBack({ 52, 33, 10, 16 });

	idleR.PushBack({ 19, 17, 10, 16 });
	idleR.PushBack({ 35, 16, 10, 16 });
	idleR.PushBack({ 51, 17, 10, 16 });

	playerL.PushBack({ 52, 33, 10, 16 });
	playerL.PushBack({ 35, 32, 10, 16 });
	playerL.PushBack({ 35, 32, 10, 16 });
	playerR.PushBack({ 35, 32, 10, 16 });
	playerR.PushBack({ 35, 32, 10, 16 });
	playerL.PushBack({ 19, 33, 10, 16 });
}

bool Player::Awake() {

	position = iPoint(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());


	return true;
}

bool Player::Start() {

	//initilize textures

	PlayerStartAnims();
	texture = app->tex->Load(parameters.attribute("texturepath").as_string());
	
	
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 8, bodyType::DYNAMIC);

	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	

	return true;
}

bool Player::Update(float dt)
{

	b2Vec2 impulse = b2Vec2_zero;
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y); 
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
	{
		currentAnimation = &idleR;
	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && jumpsAvaiable > 0) {
		//
		impulse.y -= jumpPower;
		jumpsAvaiable--;
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) 
	{
		currentAnimation = &playerL;
		playerL.Update();

		impulse.x -= acceleration;
		vel = b2Vec2(speed * dt, -GRAVITY_Y);
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		currentAnimation = &playerR;
		playerR.Update();

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
	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - 10 / 2;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - 16 / 2;
	app->render->DrawTexture(texture, position.x, position.y, &(currentAnimation->GetCurrentFrame()));

	uint w,h ;
	app->win->GetWindowSize(w, h);
	app->render->camera.x = (-position.x*app->win->GetScale()) + w/2;

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
		jumpsAvaiable = 1;
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}