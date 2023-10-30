#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

#include "Box2D/Box2D/Box2D.h"

struct SDL_Texture;
void updateCameraPosition(iSight& camara, Jugador& jugador, float cameraSpeed);

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:
	float speed = 0.2f;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;
	int pickCoinFxId;
<<<<<<< HEAD
	uint texW, texH;
	int contador = 0;
	float cameraSpeed = 0.1;
=======
>>>>>>> parent of 2a0dc6b (como talcomo talcomo talcomo talcomo talcomo talcomo talcomo talcomo talcomo talcomo tal)

private:
	float jumpPower = 1.75f;
	float acceleration = 0.2f;
	uint jumpsAvaiable = 1;

};
struct iSight {
	float x, y;
};

struct Jugador {
	float x, y;
};





#endif // __PLAYER_H__