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
#include "LevelCompletedScreen.h"

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

	app->levelCompletedScreen->active = false;
	app->levelCompletedScreen->Disable();

	app->titleScreen->titleSound = 0;

	//app->audio->PlayMusic("Assets/Music/onlymp3.to - Bully Soundtrack Main Theme-7RU7CohvsMU-192k-1704300284.ogg", 1.0f);

    app->render->camera.x = 0;
    app->render->camera.y = 0;
	introSound = app->audio->LoadFx("Assets/Music/onlymp3.to-Welcome-to-Bullworth-Bully-aTAsG7J1kwI-192k-1704299364.wav");
	app->audio->PlayFx(introSound, 0);

    return true;
}

// Called each loop iteration
bool IntroScreen::Update(float dt)
{

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->audio->UnloadFx(introSound);
		app->fadeToBlack->FadeToBlackTransition((Module*)app->introScreen, (Module*)app->titleScreen, 0);
		app->introScreen->Disable();
		app->introScreen->active = false;
		
		//plays the title screen music
		app->titleScreen->active = true;
		app->titleScreen->Enable();
		app->titleScreen->titleSound = app->audio->LoadFx("Assets/Music/onlymp3.to-Bully-Soundtrack-Main-Theme-7RU7CohvsMU-192k-1704300284.wav");
		app->audio->PlayFx(app->titleScreen->titleSound, 0);
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
