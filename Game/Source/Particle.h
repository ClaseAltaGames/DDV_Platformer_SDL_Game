#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Animation.h"
//#include "p2Point.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "SDL/include/SDL_render.h"
#include "Physics.h"

//struct Collider;

struct Particle
{
public:
	// Constructor
	Particle();

	// Copy constructor
	Particle(const Particle& p);

	// Called in Particles' Update
	// Handles the logic of the particle
	// Returns false when the particle reaches its lifetime
	bool Update();


public:
	// Defines the position in the screen
	fPoint position;

	// Defines the speed at which the particle will move (pixels per second)
	fPoint speed;

	// A set of rectangle sprites
	Animation anim;

	// The particle's collider
	//Collider* collider = nullptr;

	// Defines wether the particle is alive or not
	// Particles will be set to not alive until "spawnTime" is reached
	bool isAlive = false;

	// Defines the amout of frames this particle has been active
	// Negative values mean the particle is waiting to be activated
	int frameCount = 0;

	// Defines the total amount of frames during which the particle will be active
	uint lifetime = 0;

	bool flip = false;

	PhysBody* pbody;

	ColliderType ctype;



};

#endif //__PARTICLE_H__
