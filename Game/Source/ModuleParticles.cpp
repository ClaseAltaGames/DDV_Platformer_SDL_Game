#include "ModuleParticles.h"
#include "Particle.h"
#include "App.h"

#include "Textures.h"
#include "Render.h"
#include "Scene.h"

#include "SDL/include/SDL_timer.h"

#include <iostream>;
#include "Log.h"



using namespace std;

Particles::Particles(bool startEnabled) : Module(startEnabled)
{

	name.Create("particles");



}

Particles::~Particles()
{

}


bool Particles::Awake(pugi::xml_node& config) {


	bool ret = true;


	LOG("Loading particles");

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		particles[i] = nullptr;

	//texture = App->textures->Load("Assets/Sprites/Player/Player.png");
	//texture = app->tex->Load(config.child("mainParticleTexture").attribute("texturepath").as_string());
	texturePath = (config.child("particle").attribute("texturepath").as_string());
		

	projectile.lifetime = 100;
	projectile.speed = fPoint(1, 0);
	projectile.ctype = ColliderType::PLAYER_PROYECTILE;
	projectile.anim.PushBack({ 0,0,32,16 });

	return ret;

}


bool Particles::Start()
{
	texture = app->tex->Load(texturePath);



	return true;
}

bool Particles::CleanUp()
{
	LOG("Unloading particles");

	// Delete all remaining active particles on application exit 
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (particles[i] != nullptr)
		{
			delete particles[i];
			particles[i] = nullptr;
		}
	}

	return true;
}


/*void Particles::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		// Always destroy particles that collide
		if (particles[i] != nullptr && particles[i]->collider == c1)
		{

			if (c2->type == Collider::Type::ENEMY) {
				for (uint i = 0; i < MAX_ENEMIES; i++){
					if (App->enemy->getEnemy(i) != nullptr && c2 == App->enemy->getEnemy(i)->GetCollider()) {
						if (App->enemy->getEnemy(i)->secondFloor != App->player->isSecondFloor) {
							return;
						}
						break;
					}
				}
			}




			if (!c2->active) { return; } //SI ESA COLISION NO ESTA ACTIVA, IGNORA TODO LO DE ABAJO


			if (particles[i]->collider->type == Collider::Type::PLAYER_SHOT && !App->player->holdingGun) {
				App->particles->AddParticle(shurikenDying, particles[i]->position.x, particles[i]->position.y-3, Collider::Type::NONE);
				particles[i]->collider->pendingToDelete = true;
			}
			else if (particles[i]->collider->type == Collider::Type::PLAYER_SHOT && App->player->holdingGun) {
				App->particles->AddParticle(bulletDying, particles[i]->position.x-10, particles[i]->position.y - 14, Collider::Type::NONE);
				particles[i]->collider->pendingToDelete = true;
			}
			if (particles[i]->collider != nullptr) {
				particles[i]->collider->pendingToDelete = true;
			}
			delete particles[i];
			particles[i] = nullptr;
			break;
		}
	}
}*/


bool Particles::Update(float dt)
{


	bool ret = true;

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if (particle == nullptr)	continue;
		b2Vec2 position;

		if (particle->flip) {
			position.x = PIXEL_TO_METERS(particle->position.x);
		}
		else {
			position.x = PIXEL_TO_METERS(particle->position.x + 0.4);
		}

		position.y = PIXEL_TO_METERS(particle->position.y + 0.3);
		particle->pbody->body->SetTransform(position, particle->pbody->body->GetAngle());

		/*if (particle->collider != nullptr) {
			if (particle->collider->type == Collider::Type::PLAYER_SWORD_ULTI) {


				if ((particle->position.x * SCREEN_SIZE) + particle->anim.GetCurrentFrame().w >= App->render->camera.x + SCREEN_WIDTH * SCREEN_SIZE) {
					if (particle->speed.x >= 0) {
						particle->speed.x *= -1;
					}
				}

				if ((particle->position.x * SCREEN_SIZE) + particle->anim.GetCurrentFrame().w <= App->render->camera.x + SCREEN_WIDTH/100 * SCREEN_SIZE) {
					if (particle->speed.x <= 0) {
						particle->speed.x *= -1;
					}
				}
				if ((particle->position.y * SCREEN_SIZE) + particle->anim.GetCurrentFrame().h >= App->render->camera.y + SCREEN_HEIGHT * SCREEN_SIZE) {
					if (particle->speed.y >= 0) {
						particle->speed.y *= -1;
					}
				}
				if ((particle->position.y * SCREEN_SIZE) <= App->render->camera.y) {
					particle->position.y =  10;
					particle->speed.y *= -1;
				}
			}
		}*/


		// Call particle Update. If it has reached its lifetime, destroy it
		if (particle->Update() == false)
		{
			/*if (particle->collider != nullptr) {
				particle->collider->pendingToDelete = true;
			}*/
			particle->pbody->body->GetWorld()->DestroyBody(particle->pbody->body);

			delete particle;
			particles[i] = nullptr;
		}

	}

	return ret;
}

bool Particles::PostUpdate()
{
	bool ret = true;
	
	//Iterating all particle array and drawing any active particles
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if (particle != nullptr && particle->isAlive) {

			if (particle->flip) {
				app->render->DrawTexture(texture, particle->position.x, particle->position.y, &(particle->anim.GetCurrentFrame()));
			}
			else {
				app->render->DrawTexture(texture, particle->position.x, particle->position.y, &(particle->anim.GetCurrentFrame()));
			}

		}
	}

	return ret;
}

int Particles::AddParticle(const Particle& particle, int x, int y, uint delay)
{
	Particle* p = new Particle(particle);

	p->frameCount = -(int)delay;			// We start the frameCount as the negative delay
	p->position.x = x;						// so when frameCount reaches 0 the particle will be activated
	p->position.y = y;
	p->flip = particle.flip;
	p->pbody = app->physics->CreateRectangleSensor(p->position.x + 16, p->position.y + 16, 32, 16, bodyType::DYNAMIC);
	p->pbody->ctype = particle.ctype;

	//p->pbody->ctype = ColliderType::PLAYER_PROYECTILE;
	//Adding the particle's collider
	/*if (colliderType != Collider::Type::NONE)
		p->collider = App->collisions->AddCollider(p->anim.GetCurrentFrame(), colliderType, this);*/
	int aux = lastParticle++;
	particles[aux] = p;
	lastParticle %= MAX_ACTIVE_PARTICLES;

	return aux;

}


//int Particles::AddParticle(const Particle& particle, int x, int y, Collider::Type colliderType, uint delay)
//{
//	Particle* p = new Particle(particle);
//	
//	p->frameCount = -(int)delay;			// We start the frameCount as the negative delay
//	p->position.x = x;						// so when frameCount reaches 0 the particle will be activated
//	p->position.y = y;						
//
//	//Adding the particle's collider
//	if (colliderType != Collider::Type::NONE)
//		p->collider = App->collisions->AddCollider(p->anim.GetCurrentFrame(), colliderType, this);
//	int aux = lastParticle++;
//	particles[aux] = p;
//	lastParticle %= MAX_ACTIVE_PARTICLES;
//
//	return aux;
//
//}


int Particles::AddParticle(const Particle& particle, int x, int y, fPoint initial_speed, uint delay)
{
	Particle* p = new Particle(particle);

	p->frameCount = -(int)delay;			// We start the frameCount as the negative delay
	p->position.x = x;						// so when frameCount reaches 0 the particle will be activated
	p->position.y = y;
	p->speed = initial_speed;

	//Adding the particle's collider
	/*if (colliderType != Collider::Type::NONE)
		p->collider = App->collisions->AddCollider(p->anim.GetCurrentFrame(), colliderType, this);*/
	int aux = lastParticle++;
	particles[aux] = p;
	lastParticle %= MAX_ACTIVE_PARTICLES;

	return aux;

}
//int Particles::AddParticle(const Particle& particle, int x, int y, fPoint initial_speed, Collider::Type colliderType, uint delay)
//{
//	Particle* p = new Particle(particle);
//
//	p->frameCount = -(int)delay;			// We start the frameCount as the negative delay
//	p->position.x = x;						// so when frameCount reaches 0 the particle will be activated
//	p->position.y = y;
//	p->speed = initial_speed;
//
//	//Adding the particle's collider
//	if (colliderType != Collider::Type::NONE)
//		p->collider = App->collisions->AddCollider(p->anim.GetCurrentFrame(), colliderType, this);
//	int aux = lastParticle++;
//	particles[aux] = p;
//	lastParticle %= MAX_ACTIVE_PARTICLES;
//
//	return aux;
//
//}





void Particles::SetSpeedParticle(int position, fPoint speed) {
	Particle* particle = particles[position];
	if (particle != nullptr && particle->isAlive) {
		particle->speed = speed;
	}
}

fPoint Particles::GetPositionParticle(int position) {

	Particle* particle = particles[position];
	if (particle != nullptr && particle->isAlive) {
		return particle->position;
	}
	else {
		return fPoint(0, 0);
	}
}

fPoint Particles::GetPositionSpeed(int position) {

	Particle* particle = particles[position];
	if (particle != nullptr && particle->isAlive) {
		return particle->speed;
	}
	else {
		return fPoint(0, 0);
	}
}


void Particles::DestroyCollision(int position) {
	Particle* particle = particles[position];
	if (particle != nullptr && particle->isAlive) {
		//particle->collider->pendingToDelete = true;
	}
}
