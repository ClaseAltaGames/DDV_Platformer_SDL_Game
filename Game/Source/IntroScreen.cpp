#include "IntroScreen.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "Window.h"
#include "Audio.h"
#include "Scene.h"
#include "FadeToBlack.h"
#include "Map.h"
#include "EntityManager.h"
#include "Physics.h"
#include "TitleScreen.h"

#include "SDL/include/SDL.h"    

IntroScreen::IntroScreen(bool startEnabled) : Module(startEnabled)
{
}

// Destructor
IntroScreen::~IntroScreen()
{}

// Called before render is available
bool IntroScreen::Start()
{
    introScreenTex = app->tex->Load("Assets/Textures/introScreen1.png");

	app->titleScreen->active = false;
	app->titleScreen->Disable();

	app->audio->PlayMusic("Assets/Music/introScreenMusic.ogg", 1.0f);

    app->render->camera.x = 0;
    app->render->camera.y = 0;

    return true;
}

// Called each loop iteration
bool IntroScreen::Update(float dt)
{    
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->fadeToBlack->FadeToBlackTransition((Module*)app->introScreen, (Module*)app->titleScreen, 0);
		app->introScreen->Disable();
		app->introScreen->active = false;
	}
	
    return true;
}

bool IntroScreen::PostUpdate()
{
    app->render->DrawTexture(introScreenTex, 0, 0);

	return true;
}

// Called before quitting
bool IntroScreen::CleanUp()
{
    if(introScreenTex != nullptr)
	{
		app->tex->UnLoad(introScreenTex);
		introScreenTex = nullptr;
	}

    return true;
}
