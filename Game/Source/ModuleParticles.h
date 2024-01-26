#ifndef __MODULE_PARTICLES_H__
#define __MODULE_PARTICLES_H__

#include "Module.h"

//#include "Globals.h"
#include "Particle.h"
//#include "Collider.h"

#define MAX_ACTIVE_PARTICLES 1000

struct SDL_Texture;
struct Collider;

class Particles : public Module
{
public:
	// Constructor
	// Initializes all the particles in the array to nullptr
	Particles(bool startEnabled = true);

	//Destructor
	~Particles();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called every frame
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Called when a particle collider hits another collider
	//void OnCollision(Collider* c1, Collider* c2) override;


	// Creates a new particle and adds it to the array
	// Param particle	- A template particle from which the new particle will be created
	// Param x, y		- Position x,y in the screen (upper left axis)
	// Param delay		- Delay time from the moment the function is called until the particle is displayed in screen
	//int AddParticle(const Particle& particle, int x, int y, Collider::Type colliderType = Collider::Type::NONE, uint delay = 0);
	//int AddParticle(const Particle& particle, int x, int y, fPoint initial_speed, Collider::Type colliderType = Collider::Type::NONE, uint delay = 0);
	int AddParticle(const Particle& particle, int x, int y, uint delay = 0);
	int AddParticle(const Particle& particle, int x, int y, fPoint initial_speed, uint delay = 0);



	void SetSpeedParticle(int position, fPoint speed);
	fPoint GetPositionParticle(int position);
	fPoint GetPositionSpeed(int position);
	void DestroyCollision(int position);




private:
	// Particles spritesheet loaded into an SDL Texture
	SDL_Texture* texture = nullptr;

	// An array to store and handle all the particles
	Particle* particles[MAX_ACTIVE_PARTICLES] = { nullptr };

	// An index to the last added particle
	uint lastParticle = 0;

public:

	const pugi::char_t* texturePath;
	Particle projectile;


	Particle* particle;

};

#endif // !__Particles_H__