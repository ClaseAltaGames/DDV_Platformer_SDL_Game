#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

#include "Box2D/Box2D/Box2D.h"

struct SDL_Texture;

class WEnemies : public Entity
{
public:

	WEnemies();

	virtual ~WEnemies();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();
	
	void WEnemiesStartAnims();
	
	void OnCollision(PhysBody* physA, PhysBody* physB);
};